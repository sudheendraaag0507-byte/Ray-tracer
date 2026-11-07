#pragma once

#include "hittable.h"
#include "hittable_list.h"

class sphere : public hittable {
public:

	sphere(const point3& c, double r, const shared_ptr<material> mat) : centre(c), radius(std::fmax(0, r)) , mat(mat){}

	bool hit(const ray& r, hit_rec& rey, double t_min, double t_max)const override {
		auto a = dot(r.direction(), r.direction());
		auto h = dot(r.direction(), (centre - r.point()));
		auto c = dot((centre - r.point()), (centre - r.point())) - radius * radius;
		auto discriminant = h * h - a * c;
		
		if (discriminant < 0.0f) {
			return false;
		}

		auto root = (h - sqrt(discriminant)) / a;
		if (root <= t_min || root >= t_max) {
			root = (h + sqrt(discriminant)) / a;
			if (root <= t_min || root >= t_max) return false;
		}

		rey.t = root;
		rey.p = r.moveBy(root);
		rey.mat = mat;
		
		auto normal = (rey.p - centre) / radius;
		rey.faceout(normal, r);

		return true;
	}

private:

	point3 centre;
	double radius;
	shared_ptr<material> mat;
};

class concave_mirror : public hittable {
	public:
		concave_mirror(const point3& centre , const vec3& direction , const double radius , const double radius_mirror, const shared_ptr<material> mat) : direction(direction) , centre(centre) , mat(mat) , radius(radius) , radius_mirror(radius_mirror){}

		bool hit(const ray& r, hit_rec& rey, double t_min, double t_max) const override {
			auto constant = std::sqrt(radius * radius - radius_mirror * radius_mirror);

			vec3 oc = centre - r.point();
			auto a = dot(r.direction(), r.direction());
			auto h = dot(r.direction(), oc);
			auto c = dot(oc, oc) - radius * radius;
			auto disc = h * h - a * c;
			if (disc < 0) return false;
			auto sq = std::sqrt(disc);

			for (double root : { (h - sq) / a, (h + sq) / a }) {
				if (root < t_min || root > t_max) continue;

				point3 p = r.moveBy(root);
				if (dot(p - centre, direction) < constant) continue;   // not on the bowl

				rey.t = root;
				rey.p = p;
				rey.mat = mat;
				rey.faceout((p - centre) / radius, r);
				return true;
			}
			return false;                                              // no valid hit
		}

	private:
		vec3 direction;
		point3 centre;
		double radius;
		double radius_mirror;
		shared_ptr<material> mat;
};
