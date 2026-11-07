#pragma once
#include <memory>
#include <vector>
#include <limits>

#include "hittable.h"
#include "main_rytrc.h"

class hit_list {
	public:

	std::vector<shared_ptr<hittable>> objects;

	void object_add(shared_ptr<hittable> obj) { objects.push_back(obj); }

	bool is_hitting_any_object(const ray& r, hit_rec& rey, double t_min, double t_max)const {

		bool hit_anything = false;

		hit_rec temp_rec;

		auto max_so_far = t_max ;

		for (const auto& object : objects) {

			if(object->hit(r, temp_rec,t_min,max_so_far)){

				hit_anything = true;

				max_so_far = temp_rec.t;

				rey = temp_rec; 
			}
		}

		return hit_anything;
	}
};
