#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#include <vector>
#include <Eigen/Dense>
#include "GeometricalOptics.h"
#include "Ray.h"

using namespace Eigen;
using namespace GeometricalOptics;

namespace RayTracing {
	struct Material {
		Vector3f baseColor;

		Vector3f refractivity;

		bool rediate = false;
		bool quickReflect;
		float diffuseReflectProbability = 0;
		float reflectProbability = 1;
		float refractProbability = 0;

		// Reflectivity & refractivity loss rates
		float diffuseReflectLossRate = 1;
		float reflectLoss = 1;
		float refractLoss = 1;


		Vector3f& func(Ray& ray, const Vector3f& norm, Vector3f& color, Vector3f& res) {
			float randnum = GeometricalOptics::dis(gen);

			if (randnum < diffuseReflectProbability) {
				res = diffuseReflect(ray.direct, norm);
				color *= diffuseReflectLossRate;
			}
			else if (randnum < reflectProbability + diffuseReflectProbability) {
				res = reflect(ray.direct, norm);
				color *= reflectLoss;
			}
			else {
				ray.refractivity = (ray.refractivity == 1.0) ? refractivity[0] : 1.0 / refractivity[0];
				res = refract(ray.direct, norm, ray.refractivity);
				color *= refractLoss;
			}

			color = color.cwiseProduct(baseColor);
			return res;
		}
	};
}
#endif