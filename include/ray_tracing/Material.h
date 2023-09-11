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
		float diffuseReflectProbability = 0;
		float reflectProbability = 1;
		float refractProbability = 0;

		// Reflectivity & refractivity loss rates
		float diffuseReflectLoss = 1;
		float reflectLoss = 1;
		float refractLoss = 1;

		Material() { ; }
		Material(Vector3f baseColor) : baseColor(baseColor) { ; }


		void dielectricSurfacePropagation(Ray& ray, const Vector3f& norm) {
			float randnum = GeometricalOptics::dis(gen);

			if (0 && randnum < diffuseReflectProbability) {
				ray.direct = diffuseReflect(ray.direct, norm);
				ray.color *= diffuseReflectLoss;
			}
			else if (1 || randnum < reflectProbability + diffuseReflectProbability) {
				ray.direct = reflect(ray.direct, norm);
				ray.color *= reflectLoss;
			}
			else {
				ray.refractivity = (ray.refractivity == 1.0) ? refractivity[0] : 1.0 / refractivity[0];
				ray.direct = refract(ray.direct, norm, ray.refractivity);
				ray.color *= refractLoss;
			}

			ray.color = ray.color.cwiseProduct(baseColor);
		}
	};
}
#endif