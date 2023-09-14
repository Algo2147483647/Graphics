#ifndef GRAPHICS_PLOT_H
#define GRAPHICS_PLOT_H

#include "Image.h"

namespace Graphics {

	/* 
	 * 画等高线
	 */
	void contour(Image& image, Mat<float>& map, const int N) {
		int x_step[] = { 1, 0, 1 },
			y_step[] = { 0, 1, 1 };
		double 
			max = map.max(),
			min = map.min(),								//get the max & min of the map
			delta = (max - min) / N, 
			layer = min;

		for (int i = 0; i <= N; i++, layer += delta) {		//for N layer between max & min, get the edge of each layer
			for (int y = 0; y < map.rows - 1; y++) {		//for every point(x,y) to compute
				for (int x = 0; x < map.cols - 1; x++) {
					int flag = map.data[y * map.cols + x] >= layer ? 1 : 0;
					for (int k = 0; k < 3; k++) {			//basic unit is 2x2 matrix
						int xt = x + x_step[k],
							yt = y + y_step[k];
						if (
							(map.data[yt * map.cols + xt] >= layer ? 1 : 0) != flag
							) {
							flag = 2; break;
						}
					}
					if (flag == 2) {
						for (int k = 0; k < 3; k++) {
							int xt = x + x_step[k],
								yt = y + y_step[k];
							if (map.data[yt * map.cols + xt] >= layer) 
								drawPoint(image, xt, yt);
						}
					}
				}
			}
		}
	}

	void contour(Mat<ARGB>& image, Mat<float>& map) {
		double 
			min   = map.min(),
			delta = map.max() - min;

		for (int i = 0; i < map.size(); i++)
			image(map.i2x(i), map.i2y(i)) = colorlist((map[i] - min) / delta, 1);
	}

	void contour(Mat<ARGB>& image, Mat<float>& mapX, Mat<float>& mapY, Mat<float>& mapZ) {
		double
			minX = mapX.min(), maxX = mapX.max(),
			minY = mapY.min(), maxY = mapY.max(),
			minZ = mapZ.min(), maxZ = mapZ.max();

		for (int i = 0; i < mapX.size(); i++) {
			ARGB color = 0;
			color += (ARGB)((mapX[i] - minX) / (maxX - minX) * 0xFF) * 0x10000;
			color += (ARGB)((mapY[i] - minY) / (maxY - minY) * 0xFF) * 0x100;
			color += (ARGB)((mapZ[i] - minZ) / (maxZ - minZ) * 0xFF) * 0x1;
			image(mapX.i2x(i), mapX.i2y(i)) = color;
		}
	}

}

#endif