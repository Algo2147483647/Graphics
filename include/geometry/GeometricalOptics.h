#ifndef GEOMETERICAL_OPTICS_H
#define GEOMETERICAL_OPTICS_H

#include <Eigen/Dense>
#include <random>
#include <corecrt_math_defines.h>
using namespace Eigen;

namespace GeometricalOptics {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	/*---- 反射 ----*/
	inline Vector3f reflect(const Vector3f& RayI, const Vector3f& faceVec) {
		return (RayI - 2 * faceVec.dot(RayI) * faceVec).normalized();
	}

	/*---- 折射 ----*/
	inline Vector3f refract(const Vector3f& RayI, const Vector3f& faceVec, double rateI, double rateO) {
		double k = rateI / rateO;
		double CosI = faceVec.dot(RayI);
		double CosO = 1 - k * k * (1 - CosI * CosI);

		if (CosO < 0)		// Total reflection
			return reflect(RayI, faceVec);

		return (k * RayI + (k * CosI - std::sqrt(CosO)) * faceVec).normalized();
	}

	/*---- 漫反射 ----*/
	inline Vector3f diffuseReflect(const Vector3f& RayI, const Vector3f& faceVec) {
		double r1 = 2 * M_PI * dis(gen), r2 = dis(gen);
		Vector3f t = Vector3f::UnitX();

		if (std::abs(faceVec[0]) > 0.1)
			t = Vector3f::UnitY();

		Vector3f u = (cos(r1) * sqrt(r2) * (t.cross(faceVec)).normalized()).eval();
		Vector3f v = (sin(r1) * sqrt(r2) * (faceVec.cross(u)).normalized()).eval();

		return (sqrt(1 - r2) * faceVec + u + v).normalized();
	}

	/*---- 雾 (均匀同质) ----*/
	inline double Haze(double I, double A, double dis, double beta) {
		double t = exp(-beta * dis);
		return I * t + A * (1 - t);
	}

	inline Vector3f Haze(const Vector3f& I, const Vector3f& A, double dis, double beta) {
		double t = exp(-beta * dis);
		return t * I + (1 - t) * A;
	}
}

#endif