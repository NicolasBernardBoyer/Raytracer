#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
#include "Vec3.h"
class Geometry {
    public:
        // mandatory members
        std::string type;
        Point3 ac = {1,1,1};
        Point3 dc = {1,1,1};
        Point3 sc = {0,0,0};
        int ka = 0;
        int kd = 1;
        int ks = 0;
        int pc = 0;

        // optional members
        std::vector<std::vector<int>> transform = {{1,0,0,0},
                                {0,1,0,0},
                                {0,0,1,0},
                                {0,0,0,1}};

        // other variables
        bool visible = true;

        Geometry();
        Geometry(std::string type, Point3 ac, Point3 dc, Point3 sc, int ka, int kd, int ks, int pc, std::vector<std::vector<int>> transform = {{1,0,0,0},
                                {0,1,0,0},
                                {0,0,1,0},
                                {0,0,0,1}});
};


#endif