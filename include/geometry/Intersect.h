#ifndef INTERSECT_H
#define INTERSECT_H

#include <float.h>
#include <algorithm>
#include <complex>
#include "Matrix.h"
#include "../Tensor/Tensor.h"
#include "../Function/SolvePolynomialEquation.h"

using namespace std;

#define PI 3.141592653589

namespace Intersect {
	
extern double eps;

/*---------------- 求交点 ----------------*/
// Line
bool Segments(Mat<float>& p1, Mat<float>& p2, Mat<float>& p3, Mat<float>& p4);		// 判断线段与线段相交

// Plane
double RayPlane		(Mat<float>& raySt, Mat<float>& ray, Mat<float>& a, double b);							//求交-射线与平面
double RayPlane		(Mat<float>& raySt, Mat<float>& ray, double& A, double& B, double& C, double& D);	//求交-射线与平面(3D)
double RayCircle	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, double& R, Mat<float>& normal);	//求交-射线与圆
double RayTriangle	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);			//求交-射线与三角面
double RayPolygon	(Mat<float>& raySt, Mat<float>& ray, Mat<float>* p,  int n);							//求交-射线与多边面
double RayPlaneShape(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, Mat<float>& normal, Mat<float>& one, bool(*f)(double, double));//求交-射线与平面图案

// Body
double RayQuadric(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, Mat<float>& PInv);					//求交-射线与二次曲面
double RaySphere	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, double& R);					//求交-射线与球面
double RaySphere	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, double& R, bool(*f)(double, double));			//求交-射线与球面图案
double RayCuboid	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& p1, Mat<float>& p2, Mat<float>& p3);			//求交-射线与矩体
double RayCuboid	(Mat<float>& raySt, Mat<float>& ray, Mat<float>& pmin, Mat<float>& pmax);					//求交-射线与矩体 (轴对齐)
double RayTorus		(Mat<float>& raySt, Mat<float>& ray, Mat<float>& center, double R, double r);			//求交-射线与圆环 (轴对齐)
}

#endif