#ifndef INTERSECT_H
#define INTERSECT_H

#include <float.h>
#include <algorithm>
#include <complex>
#include <cmath>
#include <Eigen/Dense>
#include <corecrt_math_defines.h>

using namespace std;
using namespace Eigen;

constexpr float EPS = 10e-4;

namespace Intersect {
	/*#############################################################################
	*
	*						求交点
	*
	##############################################################################*/
	/*
	 * 线段、线段交点
	 */
	bool OnSegments(Vector3f& p1, Vector3f& p2, Vector3f& p3) {
		if (min(p1(1), p2(1)) <= p3(1)
			&& max(p1(1), p2(1)) >= p3(1)
			&& min(p1(2), p2(2)) <= p3(2)
			&& max(p1(2), p2(2)) >= p3(2)
			)
			return true;
		return false;
	}

	bool Segments(Vector3f& p1, Vector3f& p2, Vector3f& p3, Vector3f& p4) {
		float
			d1 = (p1(1) - p3(1)) * (p4(2) - p3(2)) - (p4(1) - p3(1)) * (p1(2) - p3(2)),
			d2 = (p2(1) - p3(1)) * (p4(2) - p3(2)) - (p4(1) - p3(1)) * (p2(2) - p3(2)),
			d3 = (p3(1) - p1(1)) * (p2(2) - p1(2)) - (p2(1) - p1(1)) * (p3(2) - p1(2)),
			d4 = (p4(1) - p1(1)) * (p2(2) - p1(2)) - (p2(1) - p1(1)) * (p4(2) - p1(2));

		if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0))
			&& ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
			return true;

		else if (d1 == 0 && OnSegments(p3, p4, p1))
			return true;

		else if (d2 == 0 && OnSegments(p3, p4, p2))
			return true;

		else if (d3 == 0 && OnSegments(p1, p2, p3))
			return true;

		else if (d4 == 0 && OnSegments(p1, p2, p4))
			return true;

		return false;
	}

	/*
	 * 射线、平面&曲面交点
	 */
	float RayPlane(const Vector3f& raySt, const Vector3f& ray, const Vector3f& a, const float b) {
		float t = a.dot(ray);
		if (t < EPS)
			return FLT_MAX;

		float d = (a.dot(raySt) - b) / t;
		return d > 0 ? d : FLT_MAX;
	}

	float RayPlane(const Vector3f& raySt, const Vector3f& ray, const float& A, const float& B, const float& C, const float& D) {
		float t = A * ray(0) + B * ray(1) + C * ray(2);
		if (t < EPS)
			return FLT_MAX;

		float d = -(A * raySt(0) + B * raySt(1) + C * raySt(2) + D) / t;
		return d > 0 ? d : FLT_MAX;
	}

	float RayQuadric(const Vector3f& raySt, const Vector3f& ray, const Vector3f& center, const Vector3f& G) {
        Vector3f rayStCenter = raySt - center;

        float A = ray.dot(G.cwiseProduct(ray)),
               B = 2 * ray.dot(G.cwiseProduct(rayStCenter)),
               C = rayStCenter.dot(G.cwiseProduct(rayStCenter)) - 1,
               Delta = B * B - 4 * A * C;

        if (Delta < 0)
            return FLT_MAX;

        Delta = sqrt(Delta);
        return (-B + (-B - Delta > 0 ? -Delta : Delta)) / (2 * A);
	}

	/*
	float RayPolynomialSurface(Vector3f& raySt, Vector3f& ray, Tensor<float, 3> & A) {
	
		int n = A.dimNum, m = A.dim[0] - 1;

		float dx = ray[0], dy = ray[1], dz = ray[2],
			x0 = raySt[0], y0 = raySt[1], z0 = raySt[2];

		complex<float>* coeff, roots;
		coeff = (complex<float>*) calloc((n + 1) * sizeof(complex<float>));
		roots = (complex<float>*) calloc(n * sizeof(complex<float>));

		while (1) {

		}

		if (n == 4)
			solveQuartic(coeff, roots);

		float minn = FLT_MAX;
		for (int i = 0; i < 4; i++) {
			if (abs(roots[i].imag()) < 10e-4 && roots[i].real() > 10e-4) {
				minn = min(minn, roots->real());
			}
		}
		return minn;
	}*/


	/*
	 * 射线、平面图案
	 */
	float RayPlaneShape(const Vector3f& raySt, const Vector3f& ray, const Vector3f& center, const Vector3f& normal, const Vector3f& yAxis, bool(*f)(float, float)) {
		float d = RayPlane(raySt, ray, normal, normal.dot(center));
		if (d == FLT_MAX)
			return FLT_MAX;

		Vector3f delta = raySt + d * ray - center;
		Vector3f tmp = delta.cross(yAxis);
		return f(delta.dot(yAxis), (tmp.dot(normal) > 0 ? 1 : -1) * tmp.norm()) ? d : FLT_MAX;
	}

	/*
	 * 射线、圆交点
	 */
	float RayCircle(const Vector3f& raySt, const Vector3f& ray, const Vector3f& center, float R, const Vector3f& normal) {
		float d = RayPlane(raySt, ray, normal, normal.dot(center));
		if (d == FLT_MAX)
			return FLT_MAX;

		Vector3f tmp = raySt + d * ray - center;
		return tmp.norm() <= R ? d : FLT_MAX;
	}

	/*
	 * 射线、三角形交点
	 */
	float RayTriangle(Vector3f& raySt, Vector3f& ray, Vector3f& p1, Vector3f& p2, Vector3f& p3) {
		Vector3f edge[2], tmp, p, q;
		edge[0] = p2 - p1;
		edge[1] = p3 - p1;

		// p & a & tmp
		p = ray.cross(edge[1]);
		float a = edge[0].dot(p);

		if (a > 0)
			tmp = raySt - p1;
		else {
			tmp = p1 - raySt;
			a = -a;
		}

		if (a < EPS)
			return FLT_MAX;								//射线与三角面平行

		// u & q & v
		float u = tmp.dot(p) / a;
		if (u < 0 || u > 1)
			return FLT_MAX;

		float v = q.dot(ray) / a;
		if (v < 0 || u + v > 1)
			return FLT_MAX;

		return q.dot(edge[1]) / a;
	}

	/*
	 * 射线、球面交点
	 */
	float RaySphere(Vector3f& raySt, Vector3f& ray, Vector3f& center, float& R) {
		Vector3f rayStCenter = raySt - center;

		float
			A = ray.dot(ray),
			B = 2 * ray.dot(rayStCenter),
			C = rayStCenter.dot(rayStCenter) - R * R,
			Delta = B * B - 4 * A * C;

		if (Delta < 0)
			return FLT_MAX;									//有无交点

		Delta = sqrt(Delta);
		return (-B + (-B - Delta > 0 ? -Delta : Delta)) / (2 * A);
	}

	/*
	 * 射线、球面图案交点
	 */
	float RaySphere(const Vector3f& raySt, const Vector3f& ray, const Vector3f& center, float R, bool(*f)(float, float)) {
		Vector3f rayStCenter = raySt - center;

		float A = ray.dot(ray),
			B = 2 * ray.dot(rayStCenter),
			C = rayStCenter.dot(rayStCenter) - R * R,
			Delta = B * B - 4 * A * C;

		if (Delta < 0)
			return FLT_MAX;

		Delta = sqrt(Delta);

		if (f != NULL) {
			float d;
			Vector3f delta;

			if ((d = (-B - Delta) / (2 * A)) > 1e-4) {
				delta = (raySt + d * ray - center).normalized();
				if (f(acos(delta[2]), atan2(delta[1], delta[0]) + (delta[1] >= 0 ? M_PI / 2 : 3 * M_PI / 2)))
					return d;
			}
			if ((d = (-B + Delta) / (2 * A)) > 1e-4) {
				delta = (raySt + d * ray - center).normalized();
				if (f(acos(delta[2]), atan2(delta[1], delta[0]) + (delta[1] >= 0 ? M_PI / 2 : 3 * M_PI / 2)))
					return d;
			}
			return FLT_MAX;
		}
		return (-B + (-B - Delta > 0 ? -Delta : Delta)) / (2 * A);
	}


	/*
	 * 射线、矩体交点
	 */
	struct OBB {
		Vector3f center;
		Vector3f extents;
		Vector3f axis[3];
	};

	bool RayCuboid(const Vector3f& ray, const Vector3f& raySt, const OBB& obb) {
		float tmin = 0.0;
		float tmax = numeric_limits<float>::max();

		Vector3f p = obb.center - raySt;

		for (int i = 0; i < 3; ++i) {
			float e = obb.axis[i].dot(p);
			float f = obb.axis[i].dot(ray);

			if (fabs(f) > 1e-6) {
				float t1 = (e + obb.extents[i]) / f;
				float t2 = (e - obb.extents[i]) / f;

				if (t1 > t2) swap(t1, t2);
				tmin = max(tmin, t1);
				tmax = min(tmax, t2);

				if (tmin > tmax) return numeric_limits<float>::max();
				if (tmax < 0) return numeric_limits<float>::max();
			}
			else if (-e - obb.extents[i] > 0 || -e + obb.extents[i] < 0) {
				return numeric_limits<float>::max();
			}
		}
		return tmin;
	}

	float RayCuboid(Vector3f& raySt, Vector3f& ray, Vector3f& pmin, Vector3f& pmax) {
		float t0 = -FLT_MAX, t1 = FLT_MAX;

		for (int dim = 0; dim < 3; dim++) {
			if (fabs(ray[dim]) < EPS && (raySt[dim] < pmin[dim] || raySt[dim] > pmax[dim])) {
				return FLT_MAX;
			}
			float
				t0t = (pmin[dim] - raySt[dim]) / ray[dim],
				t1t = (pmax[dim] - raySt[dim]) / ray[dim];
			if (t0t > t1t)
				swap(t0t, t1t);

			t0 = max(t0, t0t);
			t1 = min(t1, t1t);

			if (t0 > t1 || t1 < 0)
				return FLT_MAX;
		}
		return t0 >= 0 ? t0 : t1;
	}

}

#endif