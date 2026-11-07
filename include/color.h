#pragma once

#include <iostream>
#include <vec3.h>
#include <main_rytrc.h>

inline double linear_to_gamma(double color_value) {
    if (color_value > 0.0f) {
        return std::sqrt(color_value);
    }
    return 0.0;
}

void pixelWrite(std::ostream& out,const color& pixel) {

    auto r = linear_to_gamma(pixel.x());
    auto g = linear_to_gamma(pixel.y());
    auto b = linear_to_gamma(pixel.z());

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * clamp(r,0.0,1.0));
    int gbyte = int(255.999 * clamp(g, 0.0, 1.0));
    int bbyte = int(255.999 * clamp(b, 0.0, 1.0));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}