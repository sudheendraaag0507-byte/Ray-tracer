#include <iostream>

#include <main_rytrc.h>
#include "hittable.h"
#include "hittable_list.h"
#include "object.h"
#include "camera.h"
#include "material.h"

int main() {

	// World
    hit_list world;

    auto convex_lens_material = make_shared<dielectric>(1.33);

    auto convex_lens_object = make_shared<convex_lens>(1.33,3.0,point3(0,0,-4),vec3(0,0,-1),1.5,convex_lens_material);
    world.object_add(convex_lens_object);

    auto object_test_material = make_shared<lambertian>(color(0.5,0.3,0.2));
    auto object_test = make_shared<sphere>(point3(0, -1, -10),0.25,object_test_material);
    world.object_add(object_test);
    
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.sample_rate = 500;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lookFrom = point3(0, 0, 7);
    cam.lookAt = point3(0, 0, 0);
    cam.up_point = vec3(0, 1, 0);

    cam.focus_angle = 0.0;
    cam.focal_distance = 10.0;

    cam.render(world);

	
	return 0;
}