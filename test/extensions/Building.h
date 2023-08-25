#ifndef BUILDING_H
#define BUILDING_H

#include "../src/Modeling.h"
#include "Stair.h"
#include "Bridge.h"

namespace Building {

void Grid(Modeling& md, Modeling::Point& st, vector<double>& edgeLen, int nx, int ny, double len_x, double len_y, vector<double>& direction) {
	Modeling::Point p(3);
	double
		gridLen_x = len_x / nx,
		gridLen_y = len_y / ny;

	for (int i = 0; i <= nx; i++)
		md.Cuboid(
			p = { st[0] + i * gridLen_x, st[1] + len_y / 2, st[2] },
			edgeLen[0], len_y + edgeLen[1], edgeLen[2]
		);

	for (int i = 0; i <= ny; i++)
		md.Cuboid(
			p = { st[0] + len_y / 2, st[1] + i * gridLen_y, st[2] }, 
			len_x + edgeLen[0], edgeLen[1], edgeLen[2]
		);
}

// Random Houses
void RandomHouses(
	Modeling& md, vector<double>& rectangle, 
	double maxHeight, double minHeight, int N,
	double roadsize = 0, int sizeThreshold = 0, int randThreshold = 0
) {
	vector<double> p(4), p1(4), p2(4), t1(3), t2(3);
	vector<vector<double>> rectangleSet;
	rectangleSet.push_back(rectangle);

	for (int i = 0; i < N; i++) {
		int n = rectangleSet.size();

		for (int j = 0; j < n; j++) {
			p1 = p2 = rectangleSet[j];

			int ind = (i % 2 == 0) ? 0 : 1;

			if (rectangleSet[j][ind + 2] - roadsize > sizeThreshold) {
				double ra = rand() / (double)RAND_MAX * (1 - 2 * randThreshold) + randThreshold;
				ra *= rectangleSet[j][ind + 2];

				p1[ind + 2] = ra;
				p2[ind + 2] = rectangleSet[j][ind + 2] - ra;
				p1[ind] = rectangleSet[j][ind] - rectangleSet[j][ind + 2] / 2 + ra / 2;
				p2[ind] = rectangleSet[j][ind] + ra / 2;

				rectangleSet[j] = p1;
				rectangleSet.push_back(p2);
			}
		}
	}

	int n = rectangleSet.size();
	for (int j = 0; j < n; j++) {
		if (rectangleSet[j][2] - roadsize < sizeThreshold / 2 ||
			rectangleSet[j][3] - roadsize < sizeThreshold / 2)
			continue;

		double h = rand() / (double)RAND_MAX * (maxHeight - minHeight) + minHeight;

		md.Cuboid(
			t1 = { rectangleSet[j][0], rectangleSet[j][1], h / 2 },
			rectangleSet[j][2] - roadsize, rectangleSet[j][3] - roadsize, h
		);
	}
}

}
#endif