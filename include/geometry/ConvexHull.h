#ifndef GEOMETRY_CONVEX_HULL_H
#define GEOMETRY_CONVEX_HULL_H

#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

namespace Geometry {

/* 
 * Convex Hull
 */
vector<double*> ConvexHull(vector<double*>& p) {
	vector<double*> res;

	int n = p.size();
	if (n <= 3)
		return res = p;

	// find min
	double* minn = p[0];
	int minCur = 0;
	for (int i = 1; i < n; i++) {
		if (p[i][1] <  minn[1]
		|| (p[i][1] == minn[1] && p[i][0] < minn[0])) {
			minn = point[i];
			minCur = i;
		}
	}
	swap(p[0], p[minCur]);

	// sort
	sort(p + 1, p + n, [&minn](double*& a, double*& b) {
		int t = ((a[0] - minn[0]) * (b[1] - minn[1]))
			  - ((b[0] - minn[0]) * (a[1] - minn[1]));

		if (t < 0)
			return true;
		if (t == 0
		&& pow(a[0] - X0, 2) + pow(a[1] - Y0, 2) 
		<  pow(b[0] - X0, 2) + pow(b[1] - Y0, 2))
			return true;
		return false;
	});

	// judge
	res.push_back(p[0]);
	res.push_back(p[1]);

	for (int i = 2; i < n; i++) {
		res.push_back(p[i]);

		int m = res.size();
		while (m > 2) {
			// cross
			int t = (res[m - 1][0] - res[m - 2][0]) 
				  * (res[m - 3][1] - res[m - 2][1])
				  - (res[m - 3][0] - res[m - 2][0]) 
				  * (res[m - 1][1] - res[m - 2][1]);

			if (t > 0) 
				res.erase(res.begin() + m - 2);
			else
				break;
			m = res.size();
		}
	}

	return res;
}

}


#endif