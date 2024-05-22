#pragma once

#include "Hittable.h"
#include "HittableList.h"
#include "HelperFunctions.h"

#include <algorithm>

class bvhNode : public Hittable {
    public:
        bvhNode(const HittableList& list) : bvhNode(list.objects, 0, list.objects.size()) {}

        bvhNode(const std::vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end) {
            auto objects = srcObjects; // Create a modifiable array of source scene objs

            int axis = randomInt(0,2);
            auto comparator = (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare : boxZCompare;

            size_t objectSpan = end - start;

            if (objectSpan == 1) {
                left = right = objects[start];
            } else if (objectSpan == 2) {
                if (comparator(objects[start], objects[start+1])) {
                    left = objects[start];
                    right = objects[start+1];
                } else {
                    left = objects[start+1];
                    right = objects[start];
                }
            } else {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);

                auto mid = start + objectSpan/2;
                left = make_shared<bvhNode>(objects, start, mid);
                right = make_shared<bvhNode>(objects, mid, end);
            }

            bbox = aabb(left->boundingBox(), right->boundingBox());
        }

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            if (!bbox.hit(r, rayT))
                return false;

            bool hitLeft = left->hit(r, rayT, rec);
            bool hitRight = right->hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

            return hitLeft || hitRight;
        }

        aabb boundingBox() const override { return bbox; }

    private:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        aabb bbox;

        static bool boxCompare(
            const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex
        ) {
            return a->boundingBox().axis(axisIndex).min < b->boundingBox().axis(axisIndex).min;
        }

        static bool boxXCompare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
            return boxCompare(a, b, 0);
        }
        static bool boxYCompare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
            return boxCompare(a, b, 1);
        }
        static bool boxZCompare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
            return boxCompare(a, b, 2);
        }
};
