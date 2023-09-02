#ifndef RAY_TRACING_RAY_H
#define RAY_TRACING_RAY_H

#include <Eigen/Dense>

using namespace Eigen;

namespace RayTracing{
	class Camera {
	public:
		float width;
		float height;
		Vector3f center;
		Vector3f direct;
		Vector3f ScreenXVec;
		Vector3f ScreenYVec;

		Camera(float w, float h, Vector3f& c, Vector3f& d) :
			width(w), height(h), center(c), direct(d) {
			computeCamera();
		}

		void computeCamera() {
			ScreenYVec = Vector3f(-direct(1), direct(0), 0).normalized();
			ScreenXVec = direct.cross(ScreenYVec)
							   .normalized();
		}
	};


	class Ray {
	public:
		Vector3f origin;
		Vector3f direct;
		Vector3f color;
		float refractivity = 1.0;

		Ray() { ; }
		Ray(Vector3f& origin, Vector3f& direct) : origin(origin), direct(direct), refractivity(1.0) {
			color = Vector3f::Ones();
		}
	};
}

#endif