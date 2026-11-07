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

    auto mirror_mat = make_shared<metal>(color(0.95, 0.95, 0.95));

    world.object_add(make_shared<concave_mirror>(
        point3(0, 0, -2),     // centre of the sphere
        vec3(0, 0, -1),       // centre -> cap, away from the camera
        2.5,                  // sphere radius, so focal length is 1.25
        1.8,                  // aperture radius (fits the frame; rim plane at z = -3.74)
        mirror_mat));

    // beyond the centre of curvature: should give a small, inverted image
    auto obj2 = make_shared<sphere>(point3(-1.0, 0.3, 0.0), 0.4,
        make_shared<lambertian>(color(0.1, 0.3, 0.9)));
    world.object_add(obj2);

    auto base = make_shared<sphere>(point3(-1.0, -101.8, -4.5), 100.0,
        make_shared<lambertian>(color(0.1, 0.3, 0.9)));
    world.object_add(base);

    auto obj3 = make_shared<sphere>(point3(1.3, -0.5, 1.0), 0.35,
        make_shared<lambertian>(color(0.1, 0.7, 0.2)));
    world.object_add(obj3);

    // inside the focal length, near the mirror's pole: should give an upright, magnified image
    auto obj4 = make_shared<sphere>(point3(0.4, -0.3, -3.9), 0.15,
        make_shared<lambertian>(color(0.9, 0.1, 0.1)));
    world.object_add(obj4);

    // glass ball, off-axis so it doesn't block the centre of the bowl
    auto obj5 = make_shared<sphere>(point3(-0.9, -0.6, 2.5), 0.4,
        make_shared<dielectric>(1.5));
    world.object_add(obj5);
    
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.sample_rate = 100;
    cam.max_depth = 10;

    cam.fov = 20;
    cam.lookFrom = point3(0, 0, 10);
    cam.lookAt = point3(0, 0, 0);
    cam.up_point = vec3(0, 1, 0);

    cam.focus_angle = 0.0;
    cam.focal_distance = 10.0;

    cam.render(world);

	
	return 0;
}