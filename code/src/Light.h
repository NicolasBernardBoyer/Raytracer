#ifndef LIGHT_H
#define LIGHT_H

#include "HelperFunctions.h"

#include <string>
#include <vector>


class Light {
    public:
        // mandatory members
        std::string type;
        Point3 id = {1,1,1};
        Point3 is = {1,1,1};

        // optional members
        std::vector<std::vector<int>> transform = {{1,0,0,0},
                                {0,1,0,0},
                                {0,0,1,0},
                                {0,0,0,1}};
        unsigned int n = 0;
        bool usecenter = false;
        Point3 center = {0,0,0};
        Point3 p1, p2, p3, p4 = {0,0,0};

        // other variables
        bool use = true;

        Light();
        Light(std::string type, Point3 id, Point3 is, Point3 center = {0,0,0}, Point3 p1 = {0,0,0 }, Point3 p2 = {0,0,0 }, Point3 p3 = {0,0,0 }, Point3 p4 = {0,0,0},
                                                                                 std::vector<std::vector<int>> transform = {{1,0,0,0},
                                                                                {0,1,0,0},
                                                                                {0,0,1,0},
                                                                                {0,0,0,1}}, int n = 0, bool usecenter = false);
};

#endif

