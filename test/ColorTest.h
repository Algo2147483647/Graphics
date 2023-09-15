#include "Image.h"
#include "GraphicsIO.h"

using namespace Graphics;

void ColorTest() {
	Image img(2000, 2000);

	{  //rainbow
		for (int i = 0; i < img.rows(); i++) {
			for (int j = 0; j < 100 - 5; j++) {
				img(i, j) = spectrum(1.0 * i / img.rows(), "rainbow");
			}
		}
	}
	{  //rainbow
		for (int i = 0; i < img.rows(); i++) {
			for (int j = 100; j < 2 * 100 - 5; j++) {
				img(i, j) = spectrum(1 - 1.0 * i / img.rows(), "rainbow");
			}
		}
	}
	

	ppmWrite("C:/Users/29753/Desktop/ColorTest.ppm", img);
}