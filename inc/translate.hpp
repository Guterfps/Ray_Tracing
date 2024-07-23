
#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP

#include "hittable.hpp"

namespace RayTracing {

class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable> object, const Vec3& offset);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    AABB m_bbox;
    std::shared_ptr<Hittable> m_object;
    Vec3 m_offset;

};

inline Translate::Translate(
    std::shared_ptr<Hittable> object, const Vec3& offset) :
m_bbox(object->BoundingBox() + offset), m_object(object), m_offset(offset)
{}

bool Translate::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    Ray offset_r(ray.GetOrigin() - m_offset, ray.GetDirection(), ray.GetTime());

    if (!m_object->Hit(offset_r, ray_t, rec)) {
        return false;
    }

    rec.point += m_offset;

    return true;
}

inline AABB Translate::BoundingBox() const {
    return m_bbox;
}

}

#endif // TRANSLATE_HPP