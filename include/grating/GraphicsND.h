#ifndef GRAPHICS_ND_H
#define GRAPHICS_ND_H

#include "Graphics2D.h"
#include "Transform.h"

namespace Graphics {
	bool drawPoint      (Image& image, vector<Mat<int>>& Z_buf, vector<int>& p);
	void drawPoint		(Image& image, vector<double>& p0); 
	void drawPoint		(Image& image, double x = 0, double y = 0, double z = 0); 
	void drawLine		(Image& image, vector<Mat<int>>& Z_buf, vector<double>& st, vector<double>& ed);
	void drawLine		(Image& image, vector<double>& st, vector<double>& ed);
	void drawSuperCuboid(Image& image, vector<double>& pMin, vector<double>& pMax); 
	void drawGrid		(Image& image, vector<double>& delta, vector<double>& max, vector<double>& min); 
}

#endif