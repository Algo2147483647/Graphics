#include "RayTracing.h"
#include "Graphics2D.h"

using namespace RayTracing;
using namespace Graphics;


inline void RayTracingTest() {
	ObjectTree objTree;
	Camera camera(1000, 1000, Vector3f(500, 500, 500), Vector3f(800, 0, 0));
	vector<MatrixXf> img(3, MatrixXf(800, 800));

	{
		Material* material = new Material; 
		material->baseColor = { 1, 1, 0 };
		material->diffuseReflectProbability = 1;

		Sphere* shape = new Sphere({ 1000, 500, 800 }, 300);
		objTree.addObj(shape, material);
	}

	{
		Material* material  = new Material;
		material->baseColor = { 10, 10, 10 };
		material->rediate = 1;

		Sphere* shape = new Sphere({ 900, 100, 300 }, 200);
		objTree.addObj(shape, material);
	}

	{
		Material* material = new Material;
		material->baseColor = { 10, 10, 10 };
		material->diffuseReflectProbability = 0;
		material->reflectProbability = 0;
		material->refractivity[0] = 1.7;

		Sphere* shape = new Sphere({ 650, 500, 500 }, 100);
		objTree.addObj(shape, material);
	}

	objTree.build();
	RayTracing::debug(camera, objTree);

	RayTracing::traceRay(camera, objTree, img, 0, 600);

	Graphics::ppmWrite("C:/Users/29753/Desktop/outXY.ppm", imgXY);
	Graphics::ppmWrite("C:/Users/29753/Desktop/outYZ.ppm", imgYZ);

	Image imgout(800, 800);
	for (int i = 0; i < 800; i++) 
		for (int j = 0; j < 800; j++) {
			imgout(i, j) = RGB(
				min((int)(img[0](i, j) * 255), 255),
				min((int)(img[1](i, j) * 255), 255),
				min((int)(img[2](i, j) * 255), 255)).to_ARGB();
		}

	Graphics::ppmWrite("C:/Users/29753/Desktop/imgout.ppm", imgout);

}