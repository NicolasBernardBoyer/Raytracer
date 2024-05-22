#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "Hittable.h"
#include "Interval.h"
#include "aabb.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->boundingBox());
    }

    shared_ptr<Hittable> get(int i) const {
        return objects[i];
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto& object : objects) {
            if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
                rec.currentHit = object;
            }
        }
        return hitAnything;
    }

    aabb boundingBox() const override { return bbox; }
    private:
        aabb bbox;
};

#endif