#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <string>
#include "Color.h"

namespace Graphics {
	ARGB spectrum(double index, const string model) {
		double A = 1, R = 1, G = 1, B = 1;
		double a = index, b = 1 - a;

		if(model == "rainbow")
			HSV2RGB(index, 1.0, 1.0, R, G, B);
		}

		return RGB::to_ARGB(A, R, G, B);
	}
}
#endif