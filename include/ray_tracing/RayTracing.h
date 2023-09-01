#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <time.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <Eigen/Dense>
#include "Object.h"
#include "ThreadPool.h"

using namespace Eigen;

#define PI 3.141592653589
#define EPS 10e-4

/*#############################################################################
*
*						光线追踪  Ray Tracing
*
##############################################################################*/

namespace RayTracing {
	static int maxRayLevel = 6;
	static mutex _mutex;
	static Image imgXY, imgYZ;

	/*--------------------------------[ 渲染 ]--------------------------------
	*	[过程]:
			[1] 计算屏幕矢量、屏幕X,Y向轴
			[2] 对屏幕每个像素遍历
				[3] 计算像素矢量、光线矢量、光线追踪起点
				[4] 光线追踪算法
				[5] 基于结果绘制该像素色彩
	-------------------------------------------------------------------------*/
	class Camera {
	public:
		float width;
		float height;
		Vector3f center;
		Vector3f direct;
		Vector3f ScreenXVec;
		Vector3f ScreenYVec;

		Camera(float w, float h, Vector3f& c, Vector3f& d) :
			width(w), height(h), center(c), direct(d) {
			computeCamera();
		}

		void computeCamera() {
			ScreenYVec = Vector3f(-direct(1), direct(0), 0).normalized();
			ScreenXVec = direct.cross(ScreenYVec)
			                   .normalized();
		}
	};

	/******************************************************************************
	*						追踪光线
	*	[步骤]:
			[1] 遍历三角形集合中的每一个三角形
				[2]	判断光线和该三角形是否相交、光线走过距离、交点坐标、光线夹角
				[3] 保留光线走过距离最近的三角形的相关数据
			[4] 如果该光线等级小于设定的阈值等级
				计算三角形反射方向，将反射光线为基准重新计算
	&	[注]:distance > 1而不是> 0，是因为反射光线在接触面的精度内，来回碰自己....
	******************************************************************************/
	inline Vector3f& traceRay(const ObjectTree& objTree, Vector3f& raySt, Vector3f& ray, Vector3f& color, int level) {
		if (level > maxRayLevel) {
			return color = Vector3f(0, 0, 0);
		}

		Object* obj;
		float dis = objTree.seekIntersection(raySt, ray, obj);
		if (dis == FLT_MAX) {
			return color = Vector3f(0, 0, 0);
		}

		Vector3f raySt0 = raySt;
		raySt += dis * ray;
		
		Material* material = obj->material;
		if (material->rediate) {
			{
				unique_lock<mutex> lock(_mutex);
				Graphics::drawLine(imgXY, raySt0[0], raySt0[1], raySt[0], raySt[1]);
				Graphics::drawLine(imgYZ, raySt0[1], raySt0[2], raySt[1], raySt[2]);
			}
			return color = color.cwiseProduct(material->baseColor);
		}

		thread_local Vector3f faceVec;
		obj->shape->faceVector(raySt, faceVec);
		material->func(ray, faceVec, color, ray);
		
		traceRay(objTree, raySt, ray, color, level + 1);
		return color;
	}

	 
	inline void traceRay_thread(const Camera& camera, const ObjectTree& objTree, vector<MatrixXf>& img, int xSt, int xEd, int ySt, int yEd) {

		thread_local Vector3f sampleVec;
		thread_local Vector3f ray, raySt;
		thread_local Vector3f color;
		
		for (int y = ySt; y < yEd; y++) {
			for (int x = xSt; x < xEd; x++) {
				sampleVec = (x - img[0].rows() / 2.0 - 0.5) * camera.ScreenXVec +
					        (y - img[0].cols() / 2.0 - 0.5) * camera.ScreenYVec;
				raySt = camera.center + sampleVec;
				ray   = camera.direct + sampleVec;
				ray.normalize();
				color = Vector3f::Ones();

				traceRay(objTree, raySt, ray, color, 0);

				{
					unique_lock<mutex> lock(_mutex);
					for (int c = 0; c < 3; c++) 
						img[c](x, y) += color[c];
				}
			}
		}
	}

	inline void traceRay(const Camera& camera, const ObjectTree& objTree, vector<MatrixXf>& img, int sampleSt, int sampleEd) {
		int threadNum = 16;
		int threadSize = img[0].rows() / threadNum;
		ThreadPool pool(threadNum);
		
		for (int sample = sampleSt; sample < sampleEd; sample++) {
			for (int i = 0; i < threadNum; i++) {
				pool.enqueue([&, i] {
					traceRay_thread(camera, objTree, img, i * threadSize, (i + 1) * threadSize, 0, img[0].cols());
				});
			}
		}

		for(int i = 0; i < 3; i++) {
			img[i] *= 1.0 / (sampleEd - sampleSt);
		}
	}

	void debug(const Camera& camera, const ObjectTree& objTree) {
		Graphics::PaintSize = 1;
		imgXY = Image(1000, 1000);
		imgYZ = Image(1000, 1000);
		imgXY.setZero();
		imgYZ.setZero();

		Graphics::PaintColor = 0xFFFFFF;
		for (auto& obj : objTree.ObjectSet) {
			obj.shape->paint(imgXY, imgYZ);
		}

		{
			Graphics::PaintColor = 0xFF0000;
			Graphics::drawLine(imgXY, camera.center[0],  camera.center[1],
				                      camera.center[0] + camera.direct[0],
				                      camera.center[1] + camera.direct[1]);
			Graphics::drawLine(imgYZ, camera.center[1],  camera.center[2],
				                      camera.center[1] + camera.direct[1],
				                      camera.center[2] + camera.direct[2]);

			Graphics::PaintColor = 0x00FF00;
			Graphics::drawLine(imgXY, camera.center[0],  camera.center[1],
				                      camera.center[0] + camera.ScreenYVec[0] * 100, 
				                      camera.center[1] + camera.ScreenYVec[1] * 100);

			Graphics::PaintColor = 0x0000FF;
			Graphics::drawLine(imgYZ, camera.center[1],  camera.center[2],
				                      camera.center[1] + camera.ScreenXVec[1] * 100,
				                      camera.center[2] + camera.ScreenXVec[2] * 100);

			Graphics::PaintColor = 0xFFFF00;
			Graphics::PaintSize = 0;
		}

		Graphics::ppmWrite("C:/Users/29753/Desktop/outXY.ppm", imgXY);
		Graphics::ppmWrite("C:/Users/29753/Desktop/outYZ.ppm", imgYZ);
	}
}

#endif