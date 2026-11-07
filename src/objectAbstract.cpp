#include "hittable.h"
#include "hittable_list.h"

class sphere : public hittable {
	public:

	sphere(const point3& center, double radius) : centre(centre), radius(std::fmax(0, radius)) {}

	bool hit(const ray& r, hit_rec& rey, double t_min, double t_max)const override {
		auto a = dot(r.direction(), r.direction());
		auto h = dot(r.direction(), (centre - r.point()));
		auto c = dot((centre - r.point()), (centre - r.point())) - radius * radius;
		auto discriminant = h * h - a * c;

		if (discriminant < 0.0f) {
			return false;
		}
		
		auto root =	(h - sqrt(discriminant)) / a;
		if (root <= t_min || root >= t_max) {
			root = (h + sqrt(discriminant)) / a;
			if(root <= t_min || root >= t_max) return false;
		}
		
		rey.t = root;
		rey.p = r.moveBy(root);
		auto normal = (centre - rey.p)/5;
		rey.faceout(normal,r);

		return true;
	}

	private:

	point3 centre;
	double radius;
};