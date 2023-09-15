#include "Graphics2D.h"
#include "GraphicsIO.h"

inline void Graphics2DTest() {
	// init
	Mat<ARGB> img(2048, 2048);
	img.setZero();

	// Draw Point
	Graphics::PaintColor = 0xFFFFFF;
	for (int i = 0; i < 50; i++) {
		Graphics::PaintSize = i; 
		Graphics::drawPoint(img, 50 + i * 38, 1800);
	}

	// Draw Circle & Ellipse
	Graphics::PaintSize = 5; 
	Graphics::PaintColor = 0x66CCFF; 
	Graphics::drawEllipse(img, 1000, 1000, 500, 300);

	Graphics::PaintColor = 0xFF0000; 
	Graphics::drawCircle(img, 1000, 1000, 500);

	Graphics::PaintSize = 3; 
	Graphics::PaintColor = 0xCCFF66;
	Graphics::drawEllipse(img, 1000, 1000, 200, 500);

	// Draw & Fill drawRectangle
	Graphics::PaintColor = 0xCCFF00; 
	Graphics::drawRectangle(img, 1900, 100, 1700, 300);
	//Graphics::fillRectangle(img, 1850, 150, 1750, 250, 0xCCFF00);

	{ // Draw Polygon
		int xt[] = { 100 ,300,100 }, 
			yt[] = { 900 ,1500,1700 };
		Graphics::PaintColor = 0xCC0066; 
		Graphics::drawPolygon(img, xt, yt, 3);
	}

	{ // Draw Wave
		const int N = 100;
		int x[N], y[N];

		for (int i = 0; i < N; i++) {
			x[i] = i * 10 + 500; 
			y[i] = sin(0.02 * x[i]) * 100 + 200;
		}
		Graphics::PaintColor = 0xFF0000; 
		Graphics::drawLine(img, x, y, N);
	}
	
	// Draw Number String & String
	Graphics::drawNum(img, 1000, 1200, -1203.567);

	
	Graphics::PaintColor = 0x0099FF;  
	Graphics::FontSize = 100; 
	Graphics::drawString(img, 0, 0, "Algo~");
	
	{ // Draw Bezier Curve and Fill by Flood
		Graphics::PaintColor = 0x00EEFF; Graphics::PaintSize = 3;
		vector<vector<float>> p;
		p.push_back({ 1900 - 200 , 600 });
		p.push_back({ 1900 , 600 - 500 });
		p.push_back({ 1900 + 200 , 600 });
		p.push_back({ 1900 , 600 - 100 });
		p.push_back({ 1900 - 200 , 600 });

		Graphics::drawBezier(img, p, 600);

		Graphics::PaintColor = 0x00EEFF;
		Graphics::fillFlood(img, 1900, 502);
	}
	
	{ // Love Heart
		vector<vector<float>> p1, p2;
		
		Graphics::PaintColor = 0xFF5500; Graphics::PaintSize = 3;
		p1.push_back({1800, 1000});
		p1.push_back({ 1900, 850 });
		p1.push_back({ 2050, 1000 });
		p1.push_back({ 1800, 1230 });
		Graphics::drawBezier(img, p1, 600);

		p2.push_back({1800, 1000});
		p2.push_back({1700, 850});
		p2.push_back({1550, 1000});
		p2.push_back({1800, 1230});
		Graphics::drawBezier(img, p2, 600);

		Graphics::FontSize = 30; 
		Graphics::drawString(img, 1700, 1150, "I Love U");
	}
	/*
	{ // Rotate & Scaling & Translation
		Graphics::rotate(3.14 * 60 / 360, 500, 500);
		Graphics::scaling(0.2, 0.2, 0, 0);
		Graphics::translation(100, 50);
		Graphics::PaintSize = 5; Graphics::PaintColor = 0x66CCFF; Graphics::drawEllipse(1000, 1000, 500, 300);
		Graphics::PaintColor = 0xFF0000; Graphics::drawCircle(1000, 1000, 500);
		Graphics::PaintSize = 3; Graphics::PaintColor = 0xCCFF66; Graphics::drawEllipse(1000, 1000, 200, 500);
		Graphics::PaintColor = 0xCCFF00; Graphics::drawRectangle(1900, 100, 1600, 400);
		Graphics::fillRectangle(1800, 200, 1700, 300, 0xCCFF00);
		Graphics::TransMat.E(3);
	}*/

	{ // Translucent
		int sx = 280, sy = 250;
		
		Graphics::PaintColor = 0x8800FFFF;
		Graphics::fillRectangle(img, sx, sy, sx + 200, sy + 200);

		Graphics::PaintColor = 0x88FF00FF;
		Graphics::fillRectangle(img, sx + 500, sy + 300, sx + 700, sy + 500);

		
		Graphics::fillRectangle(img, sx + 800, sy + 800, sx + 1100, sy + 1100);

		Graphics::PaintColor = 0x8800FF00;
		Graphics::fillRectangle(img, sx + 800, sy + 1400, sx + 1100, sy + 1700);

		Graphics::PaintColor = 0x88FF0000;
		Graphics::fillRectangle(img, sx + 100, sy + 800, sx + 400, sy + 1100);

		Graphics::PaintColor = 0x880000FF;
		Graphics::fillRectangle(img, sx + 500, sy + 800, sx + 700, sy + 1100);
	}
	
	{ // Sub Graphics
		Mat<ARGB> subImg = img;

		/*
		Graphics::scaling(subImg, 0.2, 0.2, 0, 0);
		Graphics::CutSelf(subImg, 0, 0, Graphics::gWidth / 5, Graphics::gHeight / 5);
		Graphics::drawCopy(subImg, 0, 500, Graphics::Map, Graphics::gWidth, Graphics::gHeight);*/
	}

	// Save .PPM
	Graphics::ppmWrite("C:/Users/29753/Desktop/Graphics2DTest.ppm", img);
}