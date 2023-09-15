#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include "RGB.h"
#include "HSV.h"

using namespace std;

namespace Graphics {
    inline void RGB2HSV(double r, double g, double b,
                        double& h, double& s, double& v) {
        double max_val = max({ r, g, b });
        double min_val = min({ r, g, b });
        double diff = max_val - min_val;

        // Calculate value
        v = max_val;

        // Calculate hue & saturation
        if (max_val == 0.0) {
            s = 0.0;
            h = -1.0;
        }
        else {
            s = diff / max_val;

            if (r == max_val) {
                h = (g - b) / diff;
            }
            else if (g == max_val) {
                h = 2.0 + (b - r) / diff;
            }
            else {
                h = 4.0 + (r - g) / diff;
            }

            h *= 60.0;

            // Ensure hue is non-negative
            if (h < 0.0) {
                h += 360.0;
            }
        }
    }

    inline void HSV2RGB(double h, double s, double v,
                        double& r, double& g, double& b) {
        if (h == 360.0) h = 0.0;
        s = min(1.0, max(0.0, s));
        v = min(1.0, max(0.0, v));

        if (s == 0.0) {
            r = v;
            g = v;
            b = v;
        }
        else {
            int i = int(h / 60.0);
            double f = (h / 60.0) - i;
            double p = v * (1.0 - s);
            double q = v * (1.0 - s * f);
            double t = v * (1.0 - s * (1.0 - f));

            switch (i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
            }
        }
    }
}

#endif