
#include "Light.h"

Light::Light(){
    // mandatory members
    this->type = "Light";
    this->id = {1,1,1};
    this->is = {1,1,1};
    this->center = {1,1,1};

    // optional members
    this->transform = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};
    this->n = 0;
    this->usecenter = false;

    this->p1 = {0,0,0};
    this->p2 = {0,0,0};
    this->p3 = {0,0,0};
    this->p4 = {0,0,0};

    // other variables
    this->use = false;
}

Light::Light(std::string type, Point3 id, Point3 is, Point3 center, Point3 p1, Point3 p2, Point3 p3, Point3 p4, std::vector<std::vector<int>> transform, int n, bool usecenter){
    // mandatory members
    this->type = type;
    this->id = id;
    this->is = is;
    this->center = center;

    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;

    // optional members
    this->transform = transform;
    this->n = n;
    this->usecenter = usecenter;

    // other variables
    this->use = use;
}
