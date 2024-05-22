#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "HelperFunctions.h"

using std::sqrt;

class Vec3 {
	public:
		double e[3];
			
		Vec3() : e{0,0,0} {}
		Vec3(double e0, double e1, double e2) : e{e0,e1,e2} {}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		// operator functions
		// returns the negative of a vector
		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		// return the element of a vector at index i
		double operator[](int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		// adds a vector to another
		Vec3& operator+=(const Vec3& v) { 
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}
		// multiplies a vector by a double t
		Vec3& operator*=(double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}
		// divides a vector by a double t
		Vec3& operator/=(double t) {
			return *this *= 1 / t;
		}
		// gives the length of a vector
		double length() const {
			return sqrt(lengthSquared());
		}
		// calculates the squared length of a vector
		double lengthSquared() const {
			return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
		}
		bool nearZero() const {
			// Return true if vec is close to 0 in all dimensions
			auto s = 1e-8;
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		}

		static Vec3 random() {
			return Vec3(randomDouble(), randomDouble(), randomDouble());
		}

		static Vec3 random(double min, double max) {
			return Vec3(randomDouble(min,max), randomDouble(min,max), randomDouble(min,max));
		}
};

// used for geometric clarity, it's the same as Vec3
using Point3 = Vec3;

// Utility functions
// outputs a string representation of a vector
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
	return (1 / t) * v;
}
// computes the dot product of 2 vectors
inline double dot(const Vec3& u, const Vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}
// computes the cross product
inline Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unitVector(Vec3 v) {
	return v / v.length();
}

inline Vec3 randomInUnitSphere() {
	while (true) {
		auto p = Vec3::random(-1,1);
		if (p.lengthSquared() < 1)
			return p;
	}
}

inline Vec3 randomUnitVector() {
	return unitVector(randomInUnitSphere());
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
	if (dot(onUnitSphere, normal) > 0.0) // same hemisphere as normal
		return onUnitSphere;
	else
		return -onUnitSphere;
}

inline Vec3 randCosDirection(const Vec3& normal) {
	// rand pts
	double u = randomDouble(0, 1);
	double v = randomDouble(0, 1);
	double theta = 2 * pi * u;
	double r = sqrt(v);
	// hemisphere
	double x = r * cos(theta);
	double y = r * sin(theta);
	double z = sqrt(std::max(0.0, 1.0 - x*x - y*y));
	Vec3 hemisDir = Vec3(x,y,z);
	Vec3 w = normal;
	Vec3 uDir = unitVector(cross(fabs(w.x()) > 0.1 ? Vec3(0,1,0) : Vec3(1,0,0), w));
	Vec3 vDir = cross(w, uDir);
	return u * uDir + v * vDir + w * z;
}

#endif