#include <chrono>
#include <iostream>
#include "RayTracing.h"
#include "Graphics2D.h"

using namespace RayTracing;
using namespace Graphics;



inline void modeling(ObjectTree& objTree) {
	Material* material;
	{
		material = new Material({ 1, 1, 1 });
		material->diffuseReflectProbability = 0;
		material->reflectProbability = 0;
		material->refractProbability = 1;
		material->refractivity[0] = 1.7;

		objTree.add(new Cuboid({ 850, 1320, 0 }, { 1150, 1350, 300 }), material);
		objTree.add(new Cuboid({ 850, 900, 0 }, { 1150, 930, 300 }), material);
		objTree.add(new Cuboid({ 850, 850, 300 }, { 1150, 1400, 330 }), material);
		objTree.add("C:/Algo/projects/graphics/assets/teapot.stl", { 1000, 1000, 331 }, 20, material);
	}
	{
		material = new Material({ 1, 1, 0 });
		material->diffuseReflectProbability = 0.5;
		material->reflectProbability = 0.5;

		objTree.add(new Cuboid({ 900, 600, 0 }, { 1100, 630, 400 }), material);
		objTree.add(new Cuboid({ 900, 800, 0 }, { 1100, 830, 150 }), material);
		objTree.add(new Cuboid({ 900, 600, 150 }, { 1100, 830, 180 }), material);
	}

	{
		material = new Material({ 10, 10, 10 });
		material->rediate = 1;

		objTree.add(new Sphere({ 1000, 1000, 1600 }, 400), material);
	}

	{
		material = new Material({ 1, 1, 1 });
		material->diffuseReflectProbability = 0;
		material->reflectProbability = 0;
		material->refractivity[0] = 1.7;

		objTree.add(new Sphere({ 1000, 1000, 900 }, 100), material);
	}
	{
		material = new Material({ 1, 0, 0 });
		material->diffuseReflectProbability = 1;

		objTree.add(new Sphere({ 1000, 600, 500 }, 60, [](Vector3f& p) {
			float x = acos(p[2]);
			float y = atan(p[1] / p[0]) + (p[1] >= 0 ? PI / 2 : PI / 2 * 3);
			y += PI / 4;
			float o = y - ((int)(y / (PI / 2))) * (PI / 2);
			float r = 3 / 2.5 * sin(o) * cos(o) / (pow(sin(o), 3) + pow(cos(o), 3));
			if ((PI - x) < 2 * asin(r)) 
				return true; 
			return false;
		}), material);
	}
	{
		material = new Material({ 1, 1, 1 });
		material->diffuseReflectProbability = 1;

		objTree.add(new Cuboid({ 0, 0, 0 }, { 2000, 2000, 2000 }), material);
	}
}

inline void RayTracingTest() {
	ObjectTree objTree;
	Camera camera(1000, 1000, Vector3f(500, 1000, 500), Vector3f(800, 0, 0));
	vector<MatrixXf> img(3, MatrixXf(800, 800));
	
	modeling(objTree);
	objTree.build();
	RayTracing::debug(camera, objTree);

	auto start = std::chrono::high_resolution_clock::now();
	RayTracing::traceRay(camera, objTree, img, 0, 200);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time taken by traceRay: " << duration.count() << " milliseconds" << std::endl;

	Image imgout(800, 800);
	for (int i = 0; i < 800; i++) 
		for (int j = 0; j < 800; j++)
			imgout(i, j) = RGB(
				min((int)(img[0](i, j) * 255), 255),
				min((int)(img[1](i, j) * 255), 255),
				min((int)(img[2](i, j) * 255), 255)).to_ARGB();

	Graphics::ppmWrite("C:/Users/29753/Desktop/outXY.ppm", imgXY);
	Graphics::ppmWrite("C:/Users/29753/Desktop/outYZ.ppm", imgYZ);
	Graphics::ppmWrite("C:/Users/29753/Desktop/imgout.ppm", imgout);

}