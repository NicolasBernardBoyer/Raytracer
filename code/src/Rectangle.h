#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Hittable.h"
#include <cmath>
#include <vector>

class Rectangle : public Hittable {
    public:
        Rectangle()
            : Hittable()
            {
                setBoundingBox();
            }


        Rectangle(Point3 _p1, Point3 _p2, Point3 _p3, Point3 _p4, 
                    Point3 _ac, Point3 _dc, Point3 _sc, float _ka, float _kd, float _ks, float _pc)
            : Hittable(_ac, _dc, _sc, _ka, _kd,  _ks, _pc), p1(_p1), p2(_p2), p3(_p3), p4(_p4)
            {
                Q = p1;
                u = p2 - p1;
                v = p4 - p1;
                auto n = cross(u, v);
                normal = unitVector(n);
                D = dot(normal, Q);
                w = n / dot(n,n);

                setBoundingBox();
            }
        virtual void setBoundingBox() {
            bbox = aabb(Q, Q + u + v).pad();
        }
        aabb boundingBox() const override { return bbox; }

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            auto denominator = dot(normal, r.direction());
            // false if ray is parallel to plane
            if (fabs(denominator) < 1e-8)
                return false;

            // false if t is not in the ray interval
            auto t = (D - dot(normal, r.origin())) / denominator;
            if (!rayT.contains(t))
                return false;

            auto intersect = r.at(t);
            Vec3 planarHitptVec = intersect - Q;
            auto alpha = dot(w, cross(planarHitptVec, v));
            auto beta = dot(w, cross(u, planarHitptVec));

            if (!isInterior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.p = intersect;
            
            rec.setFaceNormal(r, normal);

            return true; // todo
        }
        virtual bool isInterior(double a, double b, HitRecord& rec) const {
            if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
                return false;

            rec.u = a;
            rec.v = b;
            return true;
        }
        Point3 getCenter() {
            return (u + v)/2;
        }
        
        Point3 Q;
        Vec3 u, v;
        Vec3 w;
        aabb bbox;
        Vec3 normal;
        double D;
        Point3 p1;
        Point3 p2;
        Point3 p3;
        Point3 p4;
};

#endif