#include "Output.h"

Output::Output() {
    // mandatory members
    this->filename = "default";
    this->size = {500,500};
    this->lookat = {265,330,-296};
    this->up = {0,1,0};
    this->fov = 40.0;
    this->center = {278,273,800};
    this->ai = {1,1,1};
    this->bkc = {0.5,0.5,0.5};

    // optional members
    this->globalillum = false;
    this->raysperpixel = {0,0,0};
    this->twosiderenderer = true;
    this->antialiasing = false;

    // other variables
    this->maxbounces = 10;
    this->probterminate = 0;
}

Output::Output(std::string filename, std::vector<int> size, Point3 lookat, Point3 up, double fov, Point3 center, Point3 ai, Point3 bkc, std::vector<unsigned int> raysperpixel, bool globalillum, bool twosiderender, 
bool antialiasing, int maxbounces, double probterminate) {
    // mandatory members
    this->filename = filename;
    this->size = size;
    this->lookat = lookat;
    this->up = up;
    this->fov = fov;
    this->center = center;
    this->ai = ai;
    this->bkc = bkc;

    // optional members
    this->globalillum = globalillum;
    this->raysperpixel = raysperpixel;
    this->twosiderenderer = twosiderenderer;
    this->antialiasing = antialiasing;

    // other variables
    this->maxbounces = maxbounces;
    this->probterminate = probterminate;
}