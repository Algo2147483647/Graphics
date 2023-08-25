#ifndef BUILDING_BRIDGE_H
#define BUILDING_BRIDGE_H

#include "../src/Modeling.h"

namespace Building {

inline void FourLevelInterchange(Modeling& md, Modeling::Point& c, double roadL = 3, double roadH = 0.25, double tatalL = 200, double layerH = 8) {
	Modeling::Point p1(3), p2(3), p3(3);
	double
		x_step[] = { 1, 1,-1,-1 },
		y_step[] = { 1,-1, 1,-1 };
	int N = 720;

	std::function<double(double)> func = [](double x) {
		return exp(-x) / pow(1 + exp(-x), 2);
	};

	// Straight
	{
		vector<Modeling::Point> f;

		f.push_back(p1 = { -3 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +3 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +3 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -3 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -3 / 2.0 * roadL,-roadH / 2, 0 });

		int N = 720;

		for (int k = 0; k < 4; k++) {
			vector<Modeling::Point> path;

			for (int i = 0; i < N; i++) {
				double
					a = (i / (double)N - 0.5) * 2,
					x = a * 8;

				p1 = {
					c[0],
					c[1] + a * tatalL / 2.0,
					c[2]
				};

				if (k == 1 || k == 3)
					p1[0] += 0.5 * roadL + 3 / 2.0 * roadL;
				else
					p1[0] -= 0.5 * roadL + 3 / 2.0 * roadL;

				if (k >= 2) {
					swap(p1[0], p1[1]);
					p1[2] += 3 * layerH * func(x);
				}

				path.push_back(p1);
			}
			md.Translator(path, f);
		}
	}

	// Right
	{
		vector<Modeling::Point> f;

		f.push_back(p1 = { -2 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +2 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +2 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -2 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -2 / 2.0 * roadL,-roadH / 2, 0 });

		double R = tatalL / 2.0;

		for (int k = 0; k < 4; k++) {
			vector<Modeling::Point> path;

			for (int i = 0; i < N; i++) {
				double a = 0.025 * PI + PI * 0.45 / (double)N * i;

				p1 = {
					c[0] + x_step[k] * (1.5 * roadL + R - R * sin(a)),
					c[1] + y_step[k] * (1.5 * roadL + R - R * cos(a)),
					c[2]
				};

				path.push_back(p1);
			}
			md.Translator(path, f);
		}
	}

	// Left
	{
		vector<Modeling::Point> f;
		f.push_back(p1 = { -1 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +1 / 2.0 * roadL,-roadH / 2, 0 });
		f.push_back(p1 = { +1 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -1 / 2.0 * roadL, roadH / 2, 0 });
		f.push_back(p1 = { -1 / 2.0 * roadL,-roadH / 2, 0 });

		double R = tatalL * 0.3;

		for (int k = 0; k < 4; k++) {
			vector<Modeling::Point> path;

			for (int i = 0; i < N; i++) {
				double
					a = PI / 4 + (i - N / 2) / (double)N * 2.2,
					x = (i / (double)N - 0.5) * 2 * 6;

				p1 = {
					c[0] + x_step[k] * (roadL / sqrt(2) + R / sqrt(2) - R * sin(a)),
					c[1] + y_step[k] * (roadL / sqrt(2) + R / sqrt(2) - R * cos(a)),
					c[2]
				};

				if (k == 1 || k == 2)
					p1[2] += 2 * layerH * func(x);
				else
					p1[2] += layerH * func(x);

				path.push_back(p1);
			}
			md.Translator(path, f);
		}
	}
}


}
#endif // !STAIR_H
