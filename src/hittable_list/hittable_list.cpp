
#include "hittable_list.hpp"

namespace RayTracing {


bool HittableList::Hit(const Ray& ray, 
            const Interval& ray_t,
            HitRecord& rec) const {
    HitRecord tmp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.GetMax();

    for (const auto& object : m_objects) {
        if (object->Hit(ray, Interval(ray_t.GetMin(), closest_so_far), tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = std::move(tmp_rec);
        }
    }

    return hit_anything;
}


}