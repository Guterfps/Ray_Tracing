
#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <memory>
#include <vector>

#include "hittable.hpp"

namespace RayTracing {

class HittableList : public Hittable {
public:
    HittableList();
    explicit HittableList(const std::vector<std::shared_ptr<Hittable>>& objs);

    void Clear();
    void Add(std::shared_ptr<Hittable> obj);
    
    bool Hit(const Ray& ray, 
            double ray_tmin,
            double ray_tmax,
            HitRecord& rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;

};

inline HittableList::HittableList() {}

inline HittableList::
HittableList(const std::vector<std::shared_ptr<Hittable>>& objs) :
m_objects(objs) {}

inline void HittableList::Clear() {
    m_objects.clear();
}

inline void HittableList::Add(std::shared_ptr<Hittable> obj) {
    m_objects.push_back(obj);
}

bool HittableList::Hit(const Ray& ray, 
            double ray_tmin,
            double ray_tmax,
            HitRecord& rec) const {
    HitRecord tmp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;
    std::vector<std::shared_ptr<Hittable>>::const_iterator cit;

    for (const auto& object : m_objects) {
        if (object->Hit(ray, ray_tmin, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = std::move(tmp_rec);
        }
    }

    return hit_anything;
}

}


#endif // HITTABLE_LIST_HPP