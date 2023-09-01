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
		material->baseColor = { 10, 10, 0 };
		material->rediate = 1;

		Sphere* shape = new Sphere;
		shape->center = { 900, 500, 500 };
		shape->R = 300;

		objTree.addObj(shape, material);
	}

	{/*
		Material* material  = new Material;
		material->baseColor = { 1, 0, 0 };
		material->rediate = 1;

		Sphere* shape = new Sphere;
		shape->center = { 1000, 0, 1000 };
		shape->R = 20;

		objTree.addObj(shape, material);*/
	}

	objTree.build();
	RayTracing::debug(camera, objTree);

	RayTracing::traceRay(camera, objTree, img, 0, 10);

	Graphics::ppmWrite("C:/Users/29753/Desktop/outXY.ppm", imgXY);
	Graphics::ppmWrite("C:/Users/29753/Desktop/outYZ.ppm", imgYZ);

	Image imgout(800, 800);
	for (int i = 0; i < 800; i++) 
		for (int j = 0; j < 800; j++) 
			imgout(i, j) = RGB(
				min((int)img[0](i, j) * 255, 255),
				min((int)img[1](i, j) * 255, 255),
				min((int)img[2](i, j) * 255, 255)).to_ARGB();
	Graphics::ppmWrite("C:/Users/29753/Desktop/imgout.ppm", imgout);

}