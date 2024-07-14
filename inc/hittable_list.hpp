
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
    std::vector<std::shared_ptr<Hittable>>& GetObjects();
    size_t GetSize() const;
    
    bool Hit(const Ray& ray, 
            const Interval& ray_t,
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    AABB m_bbox;
    std::vector<std::shared_ptr<Hittable>> m_objects;

};

inline HittableList::HittableList() {}

inline void HittableList::Clear() {
    m_objects.clear();
}

inline void HittableList::Add(std::shared_ptr<Hittable> obj) {
    m_objects.push_back(obj);
    m_bbox = AABB(m_bbox, obj->BoundingBox());
}

inline std::vector<std::shared_ptr<Hittable>>& HittableList::GetObjects() {
    return m_objects;
}

inline size_t HittableList::GetSize() const {
    return m_objects.size();
}

inline AABB HittableList::BoundingBox() const {
    return m_bbox;
}

}


#endif // HITTABLE_LIST_HPP