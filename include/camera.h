#pragma once
#include <iostream>
#include <fstream>
#include <hittable.h>
#include <hittable_list.h>
#include <material.h>

std::ofstream out("randomImage.ppm");
class camera {
	public:
		int image_width = 2022;
		double aspect_ratio = 16.0 / 9.0;
		double sample_rate = 10.0f;
		int max_depth = 10;
		point3 lookFrom = vec3(0.0f,0.0f,0.0f);
		point3 lookAt = vec3(0.0f,0.0f,-1.0f);
		point3 up_point = vec3(0.0f, 1.0f, 0.0f);
		double aliasing_constant ;
		double fov ;
		double focal_length = 10.0f;
		double focus_angle = 0.0f;
		double focal_distance = 1.0f;
		vec3 random_u , random_v ;

		void render(const hit_list& world) {
			initialize();
			aliasing_constant = 1.0f / sample_rate;
			out << "P3\n" << image_width << " " << image_hieght << "\n255\n";

			for (int i = 0; i < image_hieght; i++) {
				
				for (int j = 0; j < image_width; j++) {
					ray new_ray;
					color aliasing_sample(0,0,0);
					color new_color;
					
					for (int sampleNumber = 0; sampleNumber < sample_rate; sampleNumber++) {
						new_ray = get_ray_color(i,j);
						new_color = rayColor(new_ray,max_depth,world);
						aliasing_sample += new_color;
					}
					pixelWrite(out, aliasing_constant * aliasing_sample);
				}
			}

			out.close();
		}

	private:
		int image_hieght;
		point3 pixel00_loc;
		point3 cameraPosition;
		vec3 pixel_del_u;
		vec3 pixel_del_v;
		

		

		void initialize(){
			
			vec3 up_vector = up_point - lookFrom;

			//orthogonal vectors of our camera
			vec3 lookDirection = unit_vector(lookAt - lookFrom);
			vec3 u = unit_vector(cross(lookDirection, up_vector));
			vec3 v = unit_vector(cross(u, lookDirection));

			image_hieght = int(image_width / aspect_ratio);
			image_hieght = (image_hieght < 1) ? 1 : image_hieght;

			auto h = std::tan(degrees_to_radians(fov/2.0));

			auto viewport_hieght = 2.0 * h * focal_distance;
			auto viewport_width = viewport_hieght * (double(image_width) / image_hieght);

			cameraPosition = lookFrom;
			
			auto viewport_u = viewport_width*u;
			auto viewport_v = -viewport_hieght*v;

			auto top_left = cameraPosition + focal_distance*lookDirection - viewport_v / 2.0 - viewport_u / 2.0;

			pixel_del_u = viewport_u / double(image_width);
			pixel_del_v = viewport_v / double(image_hieght);

			pixel00_loc = top_left + 0.5 * (pixel_del_u) + 0.5 * (pixel_del_v);
			
			auto radius = focal_distance * std::fmax(std::tan(degrees_to_radians(focus_angle / 2.0)),0.0f);
			random_u = radius * u ;
			random_v = radius * v;
		}

		vec3 random_vector_generation() {
			auto unit_length = 1.0;
			vec3 temp_vec;
			while (true) {
				temp_vec = random_vector(-1.0,1.0);
				auto length = temp_vec.length();
				if (1e-160 < length && unit_length > length) {
					temp_vec = temp_vec/length ;
					return temp_vec;
				}
			}
		}

		vec3 random_direction(const vec3& normal) {
			auto random_vector = random_vector_generation();
			if (dot(random_vector, normal) > 0) {
				return random_vector;
			}
			else {
				return (-1)*(random_vector);
			}
		}

		color rayColor(const ray& r, int depth , const hit_list& world) {
			hit_rec temp_var;
			if(depth < 0) return color(0.0f,0.0f,0.0f);

			color attenuation;
			ray scattered;
			if (world.is_hitting_any_object(r,temp_var, 0.000001, infinity)) {
				if(temp_var.mat->scatter_ray(r,scattered,attenuation,temp_var))
					return attenuation * rayColor(scattered, depth - 1, world);
				return color(0.0f,0.0f,0.0f);

			}
			auto unit_vector = r.direction() / r.direction().length();

			auto a = (unit_vector.y() + 1.0) * 0.5;

			return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
		}

		ray get_ray_color(int i, int j) {

			point3 sample_ray_location = pixel00_loc + (j + random_double() - 0.5 )* pixel_del_u + (i + random_double() - 0.5 )* pixel_del_v;
			
			point3 random_point_on_unit_disk = random_in_unit_disk();

			vec3 random_vector = random_point_on_unit_disk.x()*random_u + random_point_on_unit_disk.y() * random_v + cameraPosition;

			ray final_ray = ray(random_vector , sample_ray_location - random_vector);
			
			return final_ray; 
		}
};
