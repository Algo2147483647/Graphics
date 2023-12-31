#ifndef RAY_TRACING_SHAPE_H
#define RAY_TRACING_SHAPE_H

#include <vector>
#include <memory>
#include <algorithm>
#include <Eigen/Dense>
#include "Graphics2D.h"

using namespace Eigen;
using namespace std;

#define EPS 1e-4f

// Base Shape class
class Shape {
public:
    std::function<bool(Vector3f&)> engraving = nullptr;

    Shape() { ; }
    Shape(std::function<bool(Vector3f&)> engraving) : engraving(engraving) { ; }

    virtual float intersect(const Vector3f& raySt, const Vector3f& ray) = 0;  // Pure virtual function
    virtual Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) = 0;
    virtual void boundingBox(Vector3f& pmax, Vector3f& pmin) = 0;
    virtual void paint(Image& imgXY, Image& imgYZ) = 0;
};

class Plane : public Shape {
public:
    float A, B, C, D;

    float intersect(const Vector3f& raySt, const Vector3f& ray) override {
        float t = A * ray(0) + B * ray(1) + C * ray(2);
        if (t < 0)
            return FLT_MAX;

        float d = -(A * raySt(0) + B * raySt(1) + C * raySt(2) + D) / t;
        return d > 0 ? d : FLT_MAX;
    }

    Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) override {
        return res;
    }

    void boundingBox(Vector3f& pmax, Vector3f& pmin) override {
        ;
    }

    void paint(Image& imgXY, Image& imgYZ) override {
        ;
    }
};

class LinearSurface : public Shape {
public:
    int order = 1;

    float intersect(const Vector3f& raySt, const Vector3f& ray) override {
        return FLT_MAX;
    }

    Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) override {
        return res;
    }

    void boundingBox(Vector3f& pmax, Vector3f& pmin) override {
        ;
    }

    void paint(Image& imgXY, Image& imgYZ) override {
        ;
    }
};

class Sphere : public Shape {
public:
    Vector3f center;
    float R = 0;

    Sphere() { ; }
    Sphere(Vector3f center, float R) : center(center), R(R) {; }
    Sphere(Vector3f center, float R, std::function<bool(Vector3f&)> engraving) : center(center), R(R), Shape(engraving){; }

    float intersect(const Vector3f& raySt, const Vector3f& ray) override {
        thread_local Vector3f rayStCenter;
        rayStCenter = raySt - center;

        float
            A = ray.dot(ray),
            B = 2 * ray.dot(rayStCenter),
            C = rayStCenter.dot(rayStCenter) - R * R,
            Delta = B * B - 4 * A * C;

        if (Delta < 0)
            return FLT_MAX;									//有无交点
        Delta = sqrt(Delta);

        float root1 = (-B - Delta) / (2 * A);
        float root2 = (-B + Delta) / (2 * A);

        if (engraving != nullptr) {
            thread_local Vector3f intersection;

            if (root1 > 0) {
                intersection = (raySt + root1 * ray - center).normalized();
                if (engraving(intersection)) {
                    return root1;
                }
            }
            if (root2 > 0) {
                intersection = (raySt + root2 * ray - center).normalized();
                if (engraving(intersection)) {
                    return root2;
                }
            }
            return FLT_MAX;
        }

        if (root1 > 0 && root2 > 0) return std::min(root1, root2);
        if (root1 > 0 || root2 > 0) return std::max(root1, root2);
        return FLT_MAX;
    }

    Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) override {
        res = intersect - center;
        res.normalize();
        return res;
    }

    void boundingBox(Vector3f& pmax, Vector3f& pmin) override {
        pmax = center + Vector3f(R, R, R);
        pmin = center - Vector3f(R, R, R);
    }

    void paint(Image& imgXY, Image& imgYZ) override {
        Graphics::drawCircle(imgXY, center[0], center[1], R);
        Graphics::drawCircle(imgYZ, center[1], center[2], R);
    }
};

class Triangle : public Shape {
public:
    Vector3f p1, p2, p3;

    Triangle() { ; }
    Triangle(Vector3f p1, Vector3f p2, Vector3f p3) : p1(p1), p2(p2), p3(p3) { ; }

    float intersect(const Vector3f& raySt, const Vector3f& ray) override {
        thread_local Vector3f edge[2], tmp, p, q;
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

        q = tmp.cross(edge[0]);
        float v = q.dot(ray) / a;
        if (v < 0 || u + v > 1)
            return FLT_MAX;

        return q.dot(edge[1]) / a;
    }

    Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) override {
        res = (p2 - p1)
              .cross(p3 - p1)
              .normalized();
        return res;
    }

    void boundingBox(Vector3f& pmax, Vector3f& pmin) override {
        for (int j = 0; j < 3; j++) {
            pmin[j] = min(p1[j], min(p2[j], p3[j]));
            pmax[j] = max(p1[j], max(p2[j], p3[j]));
        }
    }

    void paint(Image& imgXY, Image& imgYZ) override {
        Graphics::drawTriangle(imgXY, p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
        Graphics::drawTriangle(imgYZ, p1[1], p1[2], p2[1], p2[2], p3[1], p3[2]);
    }
};

class Cuboid : public Shape {
public:
    Vector3f pmin, pmax;

    Cuboid() { ; }
    Cuboid(Vector3f pmin, Vector3f pmax) : pmin(pmin), pmax(pmax) { ; }

    float intersect(const Vector3f& raySt, const Vector3f& ray) override {
        thread_local Vector3f tmin_t, tmax_t;
        tmin_t = (pmin - raySt).cwiseQuotient(ray);
        tmax_t = (pmax - raySt).cwiseQuotient(ray);

        thread_local float tmin, tmax;
        tmin = tmin_t.cwiseMin(tmax_t).maxCoeff();
        tmax = tmin_t.cwiseMax(tmax_t).minCoeff();

        if (tmin > tmax || tmax < 0)
            return FLT_MAX;
        return tmin >= 0 ? tmin : tmax;
    }

    Vector3f& faceVector(const Vector3f& intersect, Vector3f& res) override {
        if (fabs(intersect[0] - pmin[0]) < EPS)
            res = { -1, 0, 0 };
        else if (fabs(intersect[0] - pmax[0]) < EPS)
            res = { 1, 0, 0 };
        else if (fabs(intersect[1] - pmin[1]) < EPS)
            res = { 0, -1, 0 };
        else if (fabs(intersect[1] - pmax[1]) < EPS)
            res = { 0, 1, 0 };
        else if (fabs(intersect[2] - pmin[2]) < EPS)
            res = { 0, 0, -1 };
        else if (fabs(intersect[2] - pmax[2]) < EPS)
            res = { 0, 0, 1 };

        return res;
    }

    void boundingBox(Vector3f& pmax, Vector3f& pmin) override {
        pmax = this->pmax;
        pmin = this->pmin;
    }

    void paint(Image& imgXY, Image& imgYZ) override {
        Graphics::drawRectangle(imgXY, pmin[0], pmin[1], pmax[0], pmax[1]);
        Graphics::drawRectangle(imgYZ, pmin[1], pmin[2], pmax[1], pmax[2]);
    }
};

#endif