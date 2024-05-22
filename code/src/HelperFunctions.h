#ifndef HELPERS_H
#define HELPERS_H

// last time we were stuck in an infinite loop

#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <limits>
#include <memory>

// Librairies
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utilities
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() {
    // Return a random real in [0,1]
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
    // Returns a normal real in [min,max]
    return min + (max - min)*randomDouble();
}

inline int randomInt(int min, int max) {
    // random int generator
    return static_cast<int>(randomDouble(min, max+1));
}

inline double integrate(int N)
{
    double x, sum=0.0;
    for (int i = 0; i < N; ++i) {
        x = randomDouble(0,1);
        sum+=exp(sin(3*x*x));
    }
    return sum / double(N);
}

#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"
#endif