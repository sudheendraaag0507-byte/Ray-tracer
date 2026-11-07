#ifndef VEC3_H

#define VEC3_H

#include <iostream>
#include <cmath>

class vec3 {
public:

	double e[3];

	vec3() : e{ 0.0f , 0.0f , 0.0f } {};

	vec3(double x, double y, double z) {

		e[0] = x;
		e[1] = y;
		e[2] = z;

	}

	double x() const{
		return e[0];
	}

	double y() const{
		return e[1];
	}

	double z() const{
		return e[2];
	}

	double operator [] (int i) const {
		return e[i];
	}

	double dot(vec3 v1, vec3 v2) {


		return (v1[0] * v2[0] +
			v1[1] * v2[1] +
			v1[2] * v2[2]);

	}

	vec3& operator += (vec3& v1) {
		e[0] += v1[0];
		e[1] += v1[1];
		e[2] += v1[2];

		return *this;
	}

	vec3& operator *= (double v1) {
		e[0] *= v1;
		e[1] *= v1;
		e[2] *= v1;

		return *this;
	}

	vec3& operator /= (double v1) {
		e[0] /= v1;
		e[1] /= v1;
		e[2] /= v1;

		return *this;
	}

	vec3 operator - () { return vec3(-e[0], -e[1], -e[2]); }

	double& operator[](int i) { return e[i]; }

	double length() const {
		return std::sqrt(length_squared());
	}

	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	bool near_zero() {
		return (std::fabs(e[0]) < 1e-8) && (std::fabs(e[1]) < 1e-8) && (std::fabs(e[2]) < 1e-8);
	}

};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator << (std::ostream& out, const vec3& v1) {
	return out << v1.e[0] << " " << v1.e[1] << " " << v1.e[2];
}

inline vec3 operator * (const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator * (const vec3& v1, const double value) {
	return vec3(v1.e[0] * value, v1.e[1] * value, v1.e[2] * value);
}

inline vec3 operator * (const double value, const vec3& v2) {
	return vec3(value * v2.e[0], value * v2.e[1], value * v2.e[2]);
}

inline vec3 operator + (const vec3 v1, const vec3 v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator - (const vec3 v1, const vec3 v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator / (const vec3& v1, const double value) {
	return vec3(v1.e[0] / value, v1.e[1] / value, v1.e[2] / value);
}

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

inline vec3 random_vector() {
	return vec3(random_double(),random_double(), random_double());
}

inline vec3 random_vector(double min , double max) {
	return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() < 1)
			return p;
	}
}

//the inline functions are meant to be written inside header

#endif