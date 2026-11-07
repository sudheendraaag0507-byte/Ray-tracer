#pragma once
#include <main_rytrc.h>
#include <hittable.h>

static double reflectance(double cosine, double refraction_index) {

	auto r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

inline vec3 reflected_ray(const hit_rec& hit_point_data, const ray& i_ray) {
	return i_ray.direction() - 2*dot(i_ray.direction(),hit_point_data.normal) * hit_point_data.normal;
}

inline vec3 refracted_ray(const hit_rec& hit_point_data, const vec3& i_ray , const double ref_index) {
	double cos_theta = std::fmin((-1)*dot(i_ray, hit_point_data.normal),1.0);
	auto r_perpendicular = (ref_index)*(i_ray + cos_theta * hit_point_data.normal);

	auto r_parallel = (-1)*std::sqrt(1 - (r_perpendicular.length_squared())) * hit_point_data.normal;

	return r_perpendicular + r_parallel ;
}

class material {
	public:
		~material() = default;

		virtual bool scatter_ray(const ray& incident_ray, ray& scattered, color& attenuation, hit_rec& incident_ray_data) const {
			return false;
		}
};

class lambertian : public material {
	private:
		color albedo;

	public:

	lambertian(const color albedo) : albedo(albedo) {}

	bool scatter_ray(const ray& incident_ray, ray& scattered, color& attenuation, hit_rec& incident_ray_data)const override {
		auto reflected = incident_ray_data.normal + random_vector();

		if(reflected.near_zero())
			reflected = incident_ray_data.normal;

		scattered = ray(incident_ray_data.p,reflected);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
private:
	color albedo;

public:

	metal(const color albedo) : albedo(albedo) {}

	bool scatter_ray(const ray& incident_ray, ray& scattered, color& attenuation, hit_rec& incident_ray_data)const override {
		auto reflected = reflected_ray(incident_ray_data,incident_ray);
		scattered = ray(incident_ray_data.p, reflected);
		attenuation = albedo;
		return true;
	}
};

class dielectric : public material {
private:
	double refractive_index;

public:

	dielectric(const double refractive_index) : refractive_index(refractive_index) {}

	bool scatter_ray(const ray& incident_ray, ray& scattered, color& attenuation, hit_rec& incident_ray_data)const override {
		
		attenuation = color(1.0,1.0,1.0);

		auto unit_direction = unit_vector(incident_ray.direction());

		auto relative_ref_index = (incident_ray_data.out) ? 1/refractive_index : refractive_index ;

		double cos_theta = std::fmin(dot(-unit_direction, incident_ray_data.normal), 1.0);

		auto sin_theta = std::sqrt(1 - cos_theta*cos_theta);

		
		if (relative_ref_index * sin_theta > 1.0 || reflectance(cos_theta,refractive_index) > random_double()) {
			scattered = ray(incident_ray_data.p,reflected_ray(incident_ray_data, incident_ray));
		}

		else {
			scattered = ray(incident_ray_data.p,refracted_ray(incident_ray_data,unit_direction, relative_ref_index));
		}

		return true;
	}
}; 