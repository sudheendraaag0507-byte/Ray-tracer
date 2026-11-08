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

    shared_ptr<material> mat;

    auto silver = make_shared<metal>(color(0.95, 0.95, 0.95));
    auto glass = make_shared<dielectric>(1.5);

    // 1. primary mirror: focal length 6 (f/6), 1 unit wide, pole at the origin, bowl faces the sky (+z)
    world.object_add(make_shared<concave_mirror>(
        point3(0, 0, 12), vec3(0, 0, -1), 12.0, 0.5, silver));

    // 2. diagonal (flat mirror): 1.0 before the focus, turns the beam sideways to +y
    world.object_add(make_shared<plane_mirror>(
        point3(0, 0, 5), vec3(0, -1, 1), 0.15, silver));

    // 3. eyepiece: focal length 0.4, aperture radius 0.15, placed 0.4 beyond the focus at (0, 1, 5)
    world.object_add(make_shared<convex_lens>(
        1.5, 0.15, point3(0, 1.4, 5), vec3(0, 1, 0), 0.4, glass));

    // 4. distant targets (all at z = 1000)
    world.object_add(make_shared<sphere>(point3(0, 0, 1000), 6.0, make_shared<lambertian>(color(0.9, 0.15, 0.15))));   // red, on axis
    world.object_add(make_shared<sphere>(point3(10, 0, 1000), 3.0, make_shared<lambertian>(color(0.15, 0.7, 0.2))));    // green
    world.object_add(make_shared<sphere>(point3(0, -9, 1000), 2.5, make_shared<lambertian>(color(0.95, 0.8, 0.1))));    // yellow
    world.object_add(make_shared<sphere>(point3(-8, 6, 1000), 3.0, make_shared<lambertian>(color(0.15, 0.3, 0.9))));    // blue

    // camera = the eye, looking down -y into the eyepiece
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1200;          // raise to 420 once it works
    cam.sample_rate = 300;          // raise to 600 for the clean version
    cam.max_depth = 60;
    cam.fov = 34;
    cam.lookFrom = point3(0, 2.0, 5);
    cam.lookAt = point3(0, 1.4, 5);
    cam.up_point = vec3(0, 0, 1);
    cam.focal_distance = 1000;         // eye focused at infinity
    cam.focus_angle = 0.0023;       // eye pupil radius about 0.02
    cam.render(world);

	
	return 0;
}