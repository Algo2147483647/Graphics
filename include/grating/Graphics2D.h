#ifndef GRAPHICS_2D_H
#define GRAPHICS_2D_H

#include <algorithm>
#include <queue>
#include <vector>
#include <corecrt_math_defines.h>
#include "Image.h"
#include "bezier_curve.h"
#include "font.h"

using namespace std;

namespace Graphics {
	typedef long long	int64; 
	typedef float		fp32;
	typedef float		fp64;

	extern ARGB PaintColor;
	extern int  PaintSize, FontSize;

	/*-------------------------------- DRAW --------------------------------*/
	// Basic Geometry
	void drawPoint		(Image& image, int x0, int y0);								//画点
	void drawLine		(Image& image, int x1, int y1, int x2, int y2);				//画线
	void drawLine		(Image& image, int* x, int* y, int n);						//画折线
	void drawCircle		(Image& image, int x0, int y0, int r);						//画圆
	void drawEllipse	(Image& image, int x0, int y0, int rx, int ry);				//画椭圆
	void drawTriangle	(Image& image, int x1, int y1, 
										   int x2, int y2, int x3, int y3);				//画三角形
	void drawRectangle	(Image& image, int x1, int y1, int x2, int y2);				//画矩形
	void fillRectangle	(Image& image, int sx, int sy, int ex, int ey);				//填充矩形
	void drawRegularPolygon	
						(Image& image, int  x, int  y, int l, int n, float a0 = 0);//画正多边形
	void drawPolygon	(Image& image, int* x, int* y, int n);						//画多边形
	void fillPolygon	(Image& image, int* x, int* y, int n);						//填充多边形
	void drawBezier		(Image& image, vector<vector<float>>& points, int n);

	// Tessellation
	void drawGrid		(Image& image, int sx, int sy, int ex, int ey, int dx, int dy);	//画网格

	// Fill
	void fillFlood		(Image& image, int x0, int y0);								//泛滥填充

	// Text
	void drawChar		(Image& image, int x0, int y0, char charac);				//显示字符
	void drawString		(Image& image, int x0, int y0, const char* str);			//显示字符串
	void drawNum		(Image& image, int x0, int y0, fp64 num);					//显示数字
}

#endif