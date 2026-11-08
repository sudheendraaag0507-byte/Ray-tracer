#pragma once

#include "main_rytrc.h"
#include <Ray.h>


class material;

class hit_rec {
	public:
	double t;
	point3 p;
	shared_ptr<material> mat;
	shared_ptr<material> bmat;
	vec3 normal;
	bool out;

	void faceout(const vec3& norm, const ray& extr) {
		
		out = (dot(norm , extr.direction()) < 0);
		normal = (out) ? norm : -1*norm;
		if (out) {
			mat = bmat;
		}
	}

};

class hittable {
	public:

	virtual ~hittable() = default;
	virtual bool hit(const ray& r, hit_rec& rey, double t_min, double t_max) const = 0;
};