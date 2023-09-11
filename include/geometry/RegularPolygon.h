#include <vector>
#include <utility>

using namespace std;

// RegularPolygon
vector<vector<double>>& Polygon(int n, double a0 = 0, vector<vector<double>>& p) {
    double a = 2 * M_PI / n;

    for (int i = 0; i < n; i++) 
        p.push_back({ cos(a0 + a * i), sin(a0 + a * i)});

    return p;
}

