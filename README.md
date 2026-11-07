# CPU Path Tracer in C++

A Monte Carlo path tracer written from scratch in C++. I built it while following *Ray Tracing in One Weekend*, then started extending it with my own optics experiments (a concave mirror so far, a convex lens and a BVH next).


<img width="1197" height="673" alt="Screenshot 2026-11-02 213049" src="https://github.com/user-attachments/assets/dd7a7593-4cb5-4182-9a34-35a684986c44" />

*Final scene: diffuse, metal and glass spheres with reflections, refraction and depth of field.*

## Features

- Spheres, plus a `hittable` interface so new shapes plug in without touching the renderer
- Antialiasing by random sampling inside each pixel
- Diffuse (Lambertian) materials with recursive bouncing, which is what makes it a path tracer
- Gamma correction
- Metal with adjustable fuzz (roughness)
- Glass (dielectric): Snell's law, total internal reflection and Schlick's approximation
- Positionable camera with adjustable field of view
- Defocus blur (depth of field)
- Concave mirror, a spherical cap with a configurable aperture (my own addition)

## Build and run

> TODO: replace these with the real commands for this repo.

```bash
g++ -O2 -std=c++17 main.cpp -o raytracer
./raytracer            # writes a .ppm image
```

Open the `.ppm` in any PPM viewer, or convert it with `magick image.ppm image.png`.

## Example scene

```cpp
hit_list world;

auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
world.object_add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

world.object_add(make_shared<sphere>(point3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
world.object_add(make_shared<sphere>(point3(-4, 1, 0), 1.0, make_shared<lambertian>(color(0.4, 0.2, 0.1))));
world.object_add(make_shared<sphere>(point3(4, 1, 0), 1.0, make_shared<metal>(color(0.7, 0.6, 0.5))));

camera cam;
cam.aspect_ratio   = 16.0 / 9.0;
cam.image_width    = 1200;
cam.sample_rate    = 100;      // samples per pixel
cam.max_depth      = 10;       // maximum bounces per ray

cam.fov            = 20;       // vertical field of view, in degrees
cam.lookFrom       = point3(13, 2, 3);
cam.lookAt         = point3(0, 0, 0);
cam.up_point       = vec3(0, 1, 0);

cam.focus_angle    = 0.6;      // lens size, 0 gives a pinhole camera
cam.focal_distance = 10.0;     // distance to the perfectly sharp plane

cam.render(world);
```

## Performance

| Resolution | Samples per pixel | Max depth | Threads | Hardware | Render time |
|---|---|---|---|---|---|
| TODO | TODO | TODO | 1 | TODO | TODO |

Multithreaded results will be added once the OpenMP version is done.

## How it works

notes on the reasoning behind each step are in `docs/A_CPU_path_tracer_in_C.docx`. Here is the short version, in the order a ray travels.

**Render loop and viewport.** The image is a grid of RGB values written to a `.ppm` file. To avoid doing math in raw pixels, we build an imaginary viewport with its own coordinates and map every pixel to a point on it.

**Ray.** `P(t) = origin + t * direction`. Everything else is built on this formula.

**Ray-sphere intersection.** Substituting the ray into the sphere equation gives a quadratic in `t`. We keep the nearest positive root, which gives correct depth ordering.

**Surface normals and the hittable list.** The normal decides how a ray reflects or refracts. `hittable` is an abstract base class with a virtual `hit` function, and the list loops over every object, keeping the closest hit.

**Antialiasing.** Each pixel averages several random samples inside its square. Without it, edges look jagged.

**Diffuse materials and recursion.** A ray that hits a matte surface leaves in a random direction near the normal and keeps bouncing until it hits the sky or reaches the depth limit. Averaging many samples gives soft indirect light.

**Gamma correction.** Light values are linear but displays and eyes are not, so we take the square root of each channel before writing it. Without it the image looks too dark.

**Metal and fuzz.** The reflected direction is `v - 2 * dot(v, n) * n`. Fuzz adds a small random offset so rough metals look blurry.

**Glass.** Snell's law gives the refracted direction. When the ray is inside the glass the refractive index ratio is inverted. If Snell's law has no solution the ray is totally internally reflected, and Schlick's approximation adds the angle-dependent reflectivity.

**Positionable camera.** The camera gets its own basis from `lookFrom`, `lookAt` and `up_point`: `w = unit(lookFrom - lookAt)`, `u = unit(cross(up, w))`, `v = cross(w, u)`. The viewport is built from `u` and `v`, and its height comes from the field of view: `2 * tan(fov / 2) * focal_distance`.

**Defocus blur.** Each ray starts from a random point on a small lens disc of radius `focal_distance * tan(focus_angle / 2)` but still aims at the same point on the focus plane. Things at the focal distance stay sharp, and everything else blurs.

## Design notes

- **Interfaces for what varies.** Shapes and materials change, so each is an abstract class (`hittable`, `material`). The ray and vector types never change, so they stay plain structs.
- **One renderer, many shapes.** The renderer only calls `hit`, so adding a shape means writing one class.

## Extra: concave mirror

`concave_mirror(centre, direction, radius, radius_mirror, material)` is a spherical cap. It is a full sphere of the given `radius` with a plane cutting it at distance `sqrt(radius² - radius_mirror²)` from the centre along `direction`, which makes the rim of the cap exactly `radius_mirror` wide.

The hit test intersects the whole sphere and then keeps only roots on the cap side of the plane, using the hit point (`dot(p - centre, direction) >= cut_distance`). Testing the hit point rather than the ray's direction matters, because reflected and scattered rays arrive from both sides.

## Roadmap

- [ ] Multithreading with OpenMP, plus a thread-scaling table
- [ ] BVH for faster intersection (*The Next Week*)
- [ ] Importance sampling and PDFs (*The Rest of Your Life*)
- [ ] Convex lens, built as the intersection of two spheres
- [ ] GPU port (CUDA or compute shaders) with a CPU vs GPU speedup table

## Credits

Based on the *Ray Tracing in One Weekend* series by Peter Shirley, Trevor David Black and Steve Hollasch (https://raytracing.github.io/).

