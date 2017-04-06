#pragma once
#include <vector>

using namespace std;

const double EPS = 0.001;

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}

class RGBColor {
	unsigned int r, g, b;
};

class GeomObj {

};

class ThreeDVector : GeomObj {
public:
	ThreeDVector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};
	double x, y, z;
};

class ThreeDVector : GeomObj {
public:
	ThreeDVector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};
	double len() {
		return sqrt(x * x + y * y + z * z);
	}
	void normalize() {
		double l = len();
		x /= l;
		y /= l;
		z /= l;
	}
	ThreeDVector operator * (float a) {
		return ThreeDVector(a * x, a * y, a * z);
	}

	ThreeDVector operator + (ThreeDVector other) {
		return ThreeDVector(x + other.x, y + other.y, z + other.z);
	}


	ThreeDVector operator - (ThreeDVector other) {
		return ThreeDVector(x - other.x, y - other.y, z - other.z);
	}

	double x, y, z;
};

ThreeDVector v_cross_product(ThreeDVector& a, ThreeDVector& b) {
	return ThreeDVector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float v_dot_product(ThreeDVector& a, ThreeDVector& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Ray : GeomObj {
public:
	ThreeDVector start;
	ThreeDVector dir;
};

class Sphere : GeomObj {
public:
	Sphere(RGBColor color_, ThreeDVector center_) : color(color_), center(center_) {};

	pair<bool, ThreeDVector> ray_intersect(Ray ray) {
		float t0, t1; // solutions for t if the ray intersects 
		ThreeDVector L = ray.start - center;
		float a = v_dot_product(ray.dir, ray.dir);
		float b = 2 * v_dot_product(ray.dir, L);
		float c = v_dot_product(L, L) - r * r;

		if (!solveQuadratic(a, b, c, t0, t1)) 
			return{ false, center };

		if (t0 > t1) 
			std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) 
				return{ false, center }; // both t0 and t1 are negative 
		}

		return{ true, ray.start + ray.dir * t0 };
	
	}

	RGBColor color;
	ThreeDVector center;
	double r;
};

class Triangle : GeomObj {
public:
	Triangle(ThreeDVector a_, ThreeDVector b_, ThreeDVector c_) : a(a_), b(b_), c(c_) {};

	pair<bool, ThreeDVector> ray_intersect(Ray ray) {
		ThreeDVector res(0, 0, 0);
		auto N = normal_vector();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = v_dot_product(N, ray.dir);
		if (fabs(NdotRayDirection) < EPS) 
			return{ false, res }; // they are parallel so they don't intersect ! 

		// compute d parameter using equation 2
		float d = v_dot_product(N, a);

		// compute t (equation 3)
		float t = (v_dot_product(N, ray.start) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) 
			return { false, res }; // the triangle is behind 

								 // compute the intersection ThreeDVector using equation 1
		auto P = ray.start + ray.dir * t;

		// Step 2: inside-outside test

				 // edge 0
		auto edge0 = b - a;
		auto vp0 = P - a;
		auto C = v_cross_product(edge0, vp0);

		if (v_dot_product(N, C) < 0) 
			return { false, res }; // P is on the right side 

											   // edge 1
		auto edge1 = c - b;
		auto vp1 = P - b;
		C = v_cross_product(edge1, vp1);
		if (v_dot_product(N, C) < 0)  
			return { false, res }; // P is on the right side 

												// edge 2
		auto edge2 = a - c;
		auto vp2 = P - c;
		C = v_cross_product(edge2, vp2);
		if (v_dot_product(N, C) < 0) 
			return { false, res }; // P is on the right side; 

		return { true, P }; // this ray hits the triangle 
	}

	ThreeDVector normal_vector() {
		auto side1 = b - a;
		auto side2 = c - a;
		auto nv = v_cross_product(side1, side2);
		nv.normalize();
		return nv;
	}

	ThreeDVector a, b, c;

};

class Quadrilateral : GeomObj {
	ThreeDVector vertexes[4];
};

class Scene {
	vector<GeomObj> objects;

};