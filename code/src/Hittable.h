#ifndef HITTABLE_H
#define HITTABLE_H

#include "HelperFunctions.h"

#include "aabb.h"

class Hittable;

class HitRecord {
    public:
        shared_ptr<Hittable> currentHit;
        Point3 p;
        Vec3 normal;
        double u;
        double v;
        double t;
        bool twoSideHit;
        bool frontFace;

        void setFaceNormal(const Ray& r, const Point3& outwardNormal) {
            // Sets the hit record normal vector.
            // NOTE: the parameter 'outwardNormal' is assumed to have unit length

            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
};

class Hittable {
    public:
        Point3 ac;
        Point3 dc;
        Point3 sc;
        float ka;
        float kd;
        float ks;
        float pc;

        // optional members
        std::vector<std::vector<int>> transform = {{1,0,0,0},
                                {0,1,0,0},
                                {0,0,1,0},
                                {0,0,0,1}};

        // other variables
        bool visible = true;

        Hittable(){}

        Hittable(Point3 _ac, Point3 _dc, Point3 _sc, float _ka, float _kd, float _ks, float _pc) : 
        ac(_ac), dc(_dc), sc(_sc), ka(_ka), kd(_kd), ks(_ks), pc(_pc) {}

        virtual ~Hittable() = default;

        virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;

        virtual aabb boundingBox() const = 0;
};

#endif