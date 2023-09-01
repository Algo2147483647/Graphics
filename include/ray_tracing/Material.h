#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#include <vector>
#include <Eigen/Dense>
#include "GeometricalOptics.h"

using namespace Eigen;
using namespace GeometricalOptics;

#define RAND_DBL (rand() / float(RAND_MAX))

struct Material {
	Vector3f baseColor;

	Vector3f refractRate;

	bool rediate = false;
	bool quickReflect;
	float diffuseReflectProbability = 0;
	float reflectProbability = 1;
	float refractProbability = 0;

	// Reflectivity & refractivity loss rates
	float diffuseReflectLossRate = 1;
	float reflectLossRate = 1;
	float refractLossRate = 1;


	Vector3f& func(const Vector3f& ray, const Vector3f& norm, Vector3f& color, Vector3f& res) {
		float randnum = RAND_DBL;

		if (randnum < diffuseReflectProbability) {
			res = diffuseReflect(ray, norm);
			color *= diffuseReflectLossRate;
		}
		else if (randnum < reflectProbability + diffuseReflectProbability) {
			res = reflect(ray, norm);
			color *= reflectLossRate;
		}
		else {
			res = refract(ray, norm, 1, 1);
			color *= refractLossRate;
		}

		color = color.cwiseProduct(baseColor);
		return res;
	}
};

#endif