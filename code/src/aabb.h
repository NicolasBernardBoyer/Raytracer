#ifndef AABB_H
#define AABB_H

#include "HelperFunctions.h"

/*
Used by Hittable, HittableList, Sphere, Rectangle
*/

class aabb {
public:
    Interval x, y, z;

    aabb() {} // default is empty

    aabb(const Interval& ix, const Interval& iy, const Interval& iz)
        : x(ix), y(iy), z(iz) {}

    aabb(const Point3& a, const Point3& b) {
        // a and b are extremes for the bounding box, no need for min/max coord order
        x = Interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = Interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = Interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }

    aabb pad() {
        // Return AABB that has no side more narrow than a delta
        double delta = 0.0001;
        Interval new_x = (x.size() >= delta) ? x : x.expand(delta);
        Interval new_y = (x.size() >= delta) ? y : y.expand(delta);
        Interval new_z = (x.size() >= delta) ? y : y.expand(delta);

        return aabb(new_x, new_y, new_z);
    }

    aabb(const aabb& box0, const aabb& box1) {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const Ray& r, Interval rayT) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invD;
            auto t1 = (axis(a).max - orig) * invD;

            if (invD < 0)
                std::swap(t0, t1);

            if (t0 > rayT.min) rayT.min = t0;
            if (t1 < rayT.max) rayT.max = t1;

            if (rayT.max <= rayT.min)
                return false;
        }
        return true;
    }
    
};
#endif
