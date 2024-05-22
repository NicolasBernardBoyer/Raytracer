#ifndef OUTPUT_H
#define OUTPUT_H

#include "HelperFunctions.h"

#include <string>
#include <vector>

class Output {
    public:
        // mandatory members
        std::string filename;
        std::vector<int> size = {500,500};
        Point3 lookat = {265,330,-296};
        Point3 up = {0,1,0};
        double fov = 90;
        Point3 center = {278,273,800};
        Point3 ai = {1,1,1};
        Point3 bkc = {0.5,0.5,0.5};

        // optional members
        bool globalillum = false;
        std::vector<unsigned int> raysperpixel = {0,0,0};
        bool twosiderenderer = true;
        bool antialiasing = false;

        // other variables
        int maxbounces = 10;
        double probterminate = 0;

        Output();
        Output(std::string filename, std::vector<int> size, Point3 lookat, Point3 up, double fov, Point3 center, Point3 ai, Point3 bkc, std::vector<unsigned int> raysperpixel = {0,0,0}, bool globallium = false, bool twosiderender = true, bool antialiasing = false, int maxbounces = 10, double proterminate = 0);

};

#endif