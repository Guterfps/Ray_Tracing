
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
            const Interval& ray_t,
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

}


#endif // HITTABLE_LIST_HPP