#ifndef GEOMETERICAL_OPTICS_H
#define GEOMETERICAL_OPTICS_H

#include <Eigen/Dense>
#include <random>
#include <corecrt_math_defines.h>
using namespace Eigen;

namespace GeometricalOptics {
	thread_local std::mt19937 gen(std::random_device{}());
	thread_local std::uniform_real_distribution<> dis(0, 1);

	/*---- 反射 ----*/
	inline Vector3f reflect(const Vector3f& RayI, const Vector3f& faceVec) {
		return (RayI - 2 * faceVec.dot(RayI) * faceVec).normalized();
	}

	/*---- 折射 ----*/
	inline Vector3f refract(const Vector3f& RayI, const Vector3f& faceVec, float rateI_O) {  // rateI_O = rateI / rateO
		float CosI = faceVec.dot(RayI);
		float CosO = 1 - rateI_O * rateI_O * (1 - CosI * CosI);

		if (CosO < 0)		// Total reflection
			return reflect(RayI, faceVec);

		return (rateI_O * RayI + (rateI_O * CosI - std::sqrt(CosO)) * faceVec).normalized();
	}

	/*---- 漫反射 ----*/
	inline Vector3f diffuseReflect(const Vector3f& RayI, const Vector3f& faceVec) {
		float r1 = 2 * M_PI * dis(gen), r2 = dis(gen);
		thread_local Vector3f t;

		t = Vector3f::UnitX();
		if (std::abs(faceVec[0]) > 0.1)
			t = Vector3f::UnitY();

		thread_local Vector3f u, v;
		u = (cos(r1) * sqrt(r2) * (t.cross(faceVec)).normalized()).eval();
		v = (sin(r1) * sqrt(r2) * (faceVec.cross(u)).normalized()).eval();

		return (sqrt(1 - r2) * faceVec + u + v).normalized();
	}

	/*---- 雾 (均匀同质) ----*/
	inline float Haze(float I, float A, float dis, float beta) {
		float t = exp(-beta * dis);
		return I * t + A * (1 - t);
	}

	inline Vector3f Haze(const Vector3f& I, const Vector3f& A, float dis, float beta) {
		float t = exp(-beta * dis);
		return t * I + (1 - t) * A;
	}
}

#endif