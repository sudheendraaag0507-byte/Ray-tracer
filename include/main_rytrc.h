#pragma once


#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <omp.h>
#include <random>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline double clamp(double value , double min , double max) {
    if(value < min) return min;
    else if(value > max) return max;
    else return value;
}

inline double random_double(double min, double max) {
    return min + (max - min)*random_double();
}


// Common Headers

#include "color.h"
#include "Ray.h"
#include "vec3.h"

