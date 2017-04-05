#pragma once
#include <vector>

using namespace std;

class RGBColor {
	unsigned int r, g, b;
};

class GeomObj {

};

class Point : GeomObj {
	Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};
public:
	double x, y, z;
};

class ThreeDVector : GeomObj {
public:
	ThreeDVector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};
	ThreeDVector(Point a, Point b) {
		x = a.x - b.x;
		y = a.y - b.y;
		z = a.z - b.z;
	}
	double len() {
		return sqrt(x * x + y * y + z * z);
	}
	void normalize() {
		double l = len();
		x /= l;
		y /= l;
		z /= l;
	}
	double x, y, z;
};

ThreeDVector* v_cross_product(ThreeDVector* a, ThreeDVector* b) {
	return new ThreeDVector(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
}

class Ray : GeomObj {
	Point start;
	ThreeDVector dir;
};

class Sphere : GeomObj {

	RGBColor color;
	Point center;
	unsigned int r;
};

class Triangle : GeomObj {

	pair<bool, Point> ray_intersect(Ray ray) {

	}

	ThreeDVector* normal_vector() {
		auto side1 = new ThreeDVector(b, a);
		auto side2 = new ThreeDVector(c, a);
		auto nv = v_cross_product(side1, side2);
		(*nv).normalize;
		return nv;
	}

	Point a, b, c;

};

class Quadrilateral : GeomObj {
	Point vertexes[4];
};

class Scene {
	vector<GeomObj> objects;

};