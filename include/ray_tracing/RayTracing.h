#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <time.h>
#include <vector>
#include <algorithm>
#include <thread>
#include "../Graphics/RGB.h"
#include "../Graphics/GraphicsIO.h"
#include "../../../../Math/src/Math/Geometry/GeometricalOptics.h"
#include "Object.h"

using namespace GeometricalOptics;
using namespace ObjectLib;

#define PI 3.141592653589
#define EPS 10e-4
#define RAND_DBL (rand() / double(RAND_MAX))

/*#############################################################################
*
*						光线追踪  Ray Tracing
*
##############################################################################*/

namespace RayTracing {
	extern std::vector<Mat<float>>  PointLight;													//点光源集(QuickReflect专用)
	
	extern bool haze;
	extern Mat<float> haze_A;
	extern double haze_beta;
	
	extern int maxRayLevel;

	void traceRay(
		Mat<float>& center, Mat<float>& direct, double preSize,
		ObjectTree& objTree,
		Mat<float>& R, Mat<float>& G, Mat<float>& B,
		int sampleSt = 0, int sampleEd = 0x7FFFFFFF
	);

	void traceRay_(
		Mat<float>& center, Mat<float>& direct, double width, double height,
		ObjectTree& objTree,
		Mat<float>& R, Mat<float>& G, Mat<float>& B,
		int sampleSt = 0, int sampleEd = 0x7FFFFFFF
	);

	void traceRay_func(
		Mat<float>* ScreenXVec, Mat<float>* ScreenYVec, Mat<float>* center, Mat<float>* direct, 
		ObjectTree* objTree, double rate, 
		Mat<float>* R, Mat<float>* G, Mat<float>* B, 
		int st, int ed
	);

	Mat<float>& traceRay(ObjectTree& objTree, Mat<float>& RaySt, Mat<float>& Ray, Mat<float>& color, int level);				//追踪光线
}

#endif