#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../external/json.hpp"

#include "HelperFunctions.h"

#include "Output.h"

#include "HittableList.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Light.h"
#include "Camera.h"
#include "Color.h"
#include "bvh.h"

#include <vector>
#include <cmath>
#include <limits>
#include <memory>

// Librairies
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// RayTracer class
class RayTracer {
public:
    //HittableList world;
    std::vector<Output> out;
    HittableList world;
    std::vector<Sphere> spheres;
    std::vector<Rectangle> rectangles;
    std::vector<Light> light;

    RayTracer(nlohmann::json& j);
    void parseWorld(nlohmann::json& j);
    void parseOutput(nlohmann::json& j);
    void parseLight(nlohmann::json& j);

    void run();
};

#endif