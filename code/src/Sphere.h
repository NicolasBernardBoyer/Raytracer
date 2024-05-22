#ifndef Sphere_H
#define Sphere_H

#include "Vec3.h"
#include "Hittable.h"
#include "aabb.h"

#include <string>
#include <vector>

class Sphere : public Hittable {
    public:
        Sphere(Point3 _center, double _radius, Point3 _ac, Point3 _dc, Point3 _sc, float _ka, float _kd, float _ks, float _pc)
         : Hittable(_ac, _dc, _sc, _ka, _kd,  _ks, _pc),
          center(_center),
          radius(_radius)
        {
            auto rvec = Vec3(radius, radius, radius);
            bbox = aabb(center - rvec, center + rvec);
        }

        Sphere(Point3 _center, double _radius) : center(_center), radius(_radius)
        {
            auto rvec = Vec3(radius, radius, radius);
            bbox = aabb(center - rvec, center + rvec);
        } 

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            Vec3 oc = r.origin() - center;
            auto a = r.direction().lengthSquared();
            auto halfB = dot(oc, r.direction());
            auto c = oc.lengthSquared() - radius*radius;

            auto discriminant = halfB*halfB - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find nearest root lying in the acceptable range
            auto root = (-halfB - sqrtd) / a;
            if (!rayT.surrounds(root)) {
                root = (-halfB + sqrtd) / a;
                if (!rayT.surrounds(root))
                    return false;
            }
            rec.t = root;
            rec.p = r.at(rec.t);
                
            Point3 outwardNormal = (rec.p - center) / radius;
            rec.setFaceNormal(r, outwardNormal);

            return true;
        }
        aabb boundingBox() const override { return bbox; }

    private:
        Point3 center;
        double radius;
        //shared_ptr<Material> mat;
        // Point3 ac;
        // Point3 dc;
        // Point3 sc;
        // float ka;
        // float kd;
        // float ks;
        // float pc;
        aabb bbox;
};

#endif