﻿/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef GRAPHICS_ND_H
#define GRAPHICS_ND_H
#include "Graphics.h"
#include "GraphicsFileCode.h"
#include <conio.h>

class GraphicsND
{
public:
	/*---------------- 基础参数 ----------------*/
	Graphics g;																//核心图形学类
	Mat<Mat<int>> Z_Buffer;
	static Mat<float> TransformMat;												//变换矩阵
	static unsigned int FaceColor;
	unsigned int(*FaceColorF)(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3) = FaceColorF_1;
	double perspective = 0;
	std::vector<Mat<float>> LineSet, TriangleSet;
	bool FACE = true, LINE = false,
		 isLineTriangleSet = false;
	/*---------------- 底层 ----------------*/
   ~GraphicsND() { ; }														//析构函数
	GraphicsND(int width = 500, int height = 500, int Dim = 3) { init(width, height , Dim); }	//构造函数
	void init (int width, int height, int Dim = 3);							//初始化
	void clear(ARGB color);													//清屏
	void value2pix	(double x0, double y0, double z0, int& x, int& y, int& z);//点To像素 (<=3D)
	void value2pix	(Mat<float>& p0, Mat<int>& pAns);							//点To像素 (anyD)
	bool setPix		(int x, int y, int z = 0, int size = -1, unsigned int color = 0);	//写像素 (<=3D)
	bool setPix		(Mat<int>& p0,            int size = -1, unsigned int color = 0);	//写像素 (anyD)
	void setAxisLim	(Mat<float>& pMin, Mat<float>& pMax);								//设置坐标范围
	void writeModel (const char* fileName);									//写模型文件
	static unsigned int FaceColorF_1(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);
	static unsigned int FaceColorF_2(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);
	static unsigned int FaceColorF_3(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);
	/*---------------- DRAW ----------------*/
	// 0-D
	void drawPoint		(double x0 = 0, double y0 = 0, double z0 = 0);		//画点 (<=3D)
	void drawPoint		(Mat<float>& p0);										//画点 (anyD)
	// 1-D
	void drawLine		(double sx0 = 0, double ex0 = 0, 
						 double sy0 = 0, double ey0 = 0, 
						 double sz0 = 0, double ez0 = 0);					//画直线 (<=3D)
	void drawLine		(Mat<float>& sp0, Mat<float>& ep0);							//画直线 (anyD)
	void drawPolyline	(Mat<float>* p, int n, bool close = false);				//画折线
	void drawBezierLine	(Mat<float> p[], int n);									//画Bezier曲线
	// 2-D
	void drawTriangle	(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);					//画三角形
	void drawTriangleSet(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);					//画三角形集
	void drawTriangleSet(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3, Mat<float>&FaceVec);	//画三角形集
	void drawRectangle	(Mat<float>& sp, Mat<float>& ep, Mat<float>* direct = NULL);		//画矩形
	void drawQuadrangle	(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3, Mat<float>& p4);		//画四边形
	void drawPolygon	(Mat<float> p[], int n);									//画多边形
	void drawCircle		(Mat<float>& center, double r, double delta = 36,								 Mat<float>* direct = NULL);		//画圆
	void drawSector		(Mat<float>& center, double r, double angleSt, double angleEd, double delta = 36, Mat<float>* direct = NULL);		//画扇形
	void drawEllipse	(Mat<float>& center, double rx, double ry,										 Mat<float>* direct = NULL);		//画椭圆
	void drawSurface	(Mat<float>& z, double xs, double xe, double ys, double ye, Mat<float>* direct = NULL);		//画曲面
	void drawBezierFace	(Mat<float> p[], int n);									//画Bezier曲面
	// 3-D
	void drawTetrahedron(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3, Mat<float>& p4);		//画四面体
	void drawCuboid		(Mat<float>&pMin,Mat<float>& pMax);							//画矩体
	void drawPolyhedron	(Mat<float>* p, int n);									//画多面体
	void drawGoldbergPolyhedron	(Mat<float>&, int m, int n);						//画Goldberg多面体(五,六边形密铺球)
	void drawFrustum	(Mat<float>& st, Mat<float>& ed, double Rst, double Red, double delta = 36);	//画圆台
	void drawCylinder	(Mat<float>& st, Mat<float>& ed, double r, double delta = 36);//画圆柱
	void drawSphere		(Mat<float>& center, double r, double dAngle = 2 * M_PI / 36);			//画球
	void drawSphere		(Mat<float>& center, double r, double thetaSt, double thetaEd, 
							double phiSt = -M_PI / 2, double phiEd = M_PI / 2, double dAngle = 2*M_PI/36);//画部分球
	void drawSphere2	(Mat<float>& center, double r, int n = 300);				//画球
	void drawEllipsoid	(Mat<float>& center, Mat<float>& r);							//画椭球
	void drawBody		(Mat<float>& center, Mat<float>& r);							//画曲体
	void drawBezierBody	(Mat<float> p[], int n);									//画Bezier曲体
	void drawPipe		(Mat<float>& st, Mat<float>& ed, double Rst, double Red, int delta = 36);	//画平移体(粗细正多边形截面,线段路径)
	void drawPipe		(Mat<float>& st, Mat<float>& ed, double R,               int delta = 36);	//画平移体(正多边形截面,线段路径)
	void drawPipe		(Mat<float>* p,  int N,	   double R,			   int delta = 36);	//画平移体(正多边形截面,any路径)
	void drawPipe		(Mat<float>& path,		   double R,			   int delta = 36);	//画平移体(正多边形截面,any路径)
	void drawPipe		(Mat<float>& st, Mat<float>& ed, Mat<float>& f);								//画平移体(any截面,线段路径)
	void drawPipe		(Mat<float>& path, Mat<float>& f);										//画平移体(any截面,any路径)
	void drawRotator	(Mat<float>& zero, Mat<float>& axis, Mat<float>& f, int delta = 36, double st = 0, double ed = 2 * M_PI);	//画旋转体
	void drawStairs		(Mat<float>& zero, double Length, double Width, double Height, int Num);	//画阶梯
	// Word
	void drawChar		(Mat<float>& p0, char charac);				//显示字符
	void drawString		(Mat<float>& p0, const char* str);			//显示字符串
	void drawNum		(Mat<float>& p0, double num);				//显示数字
	// any-D
	void drawSuperLine	(Mat<float>* p0);							//画线 any-D
	void drawSuperCuboid(Mat<float>& pMin, Mat<float>& pMax);				//画立方体 any-D
	void drawSuperSphere(Mat<float>& center, double r);				//画球体 any-D
	void draw4DSphere(Mat<float>& center, double r);				//画球体 any-D
	void drawGrid		(Mat<float>& delta, Mat<float>& max, Mat<float>& min);	//画网格
	// Other
	void drawAxis(double Xmax = 0, double Ymax = 0, double Zmax = 0, bool negative = false);						//画坐标轴
	void contour	(Mat<float>& map, const int N);																		//画等高线
	void contour	(Mat<float>& map);																					//画等高面
	void contour	(Mat<float>& mapX, Mat<float>& mapY, Mat<float>& mapZ);
	ARGB colorlist(double index, int model = 1);																	//色谱
	/*---------------- 几何变换 Transformation ----------------*/
	Mat<float>& translate(Mat<float>& delta,									   Mat<float>& transMat = TransformMat);	//平移
	Mat<float>& rotate	(Mat<float>& theta,						Mat<float>& center, Mat<float>& transMat = TransformMat);	//旋转
	Mat<float>& rotate	(Mat<float>& rotateAxis, double theta,	Mat<float>& center, Mat<float>& transMat = TransformMat);	//旋转 3D
	Mat<float>& scale	(Mat<float>& ratio,						Mat<float>& center, Mat<float>& transMat = TransformMat);	//缩放
	Mat<float>& reflect	(Mat<float>& e,							Mat<float>& center, Mat<float>& transMat = TransformMat);	//镜像
	Mat<float>& perspect (Mat<float>& e, Mat<float>& center, Mat<float>& transMat); // 透视投影
	/*---------------- 交互 ----------------*/
	void interactive();
};
#endif