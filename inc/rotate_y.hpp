
#ifndef ROTATE_Y_HPP
#define ROTATE_Y_HPP

#include "hittable.hpp"

namespace RayTracing {

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;

private:
    AABB m_bbox;
    std::shared_ptr<Hittable> m_object;
    double m_sin_theta;
    double m_cos_theat;

};

inline RotateY::RotateY(std::shared_ptr<Hittable> object, double angle) 

{}

inline bool RotateY::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    Point3 origin = ray.GetOrigin();
    Vec3 direction = ray.GetDirection();

    origin[Vec3::Cord::X] = m_cos_theta * ray.GetOrigin().GetX() - 
                            m_sin_theta * ray.GetOrigin().GetZ();
    origin[Vec3::Cord::Z] = m_sin_theta * ray.GetOrigin().GetX() + 
                            m_cos_theta * ray.GetOrigin().GetZ();
    direction[Vec3::Cord::X] = m_cos_theta * ray.GetOrigin().GetX() - 
                            m_sin_theta * ray.GetOrigin().GetZ();
    direction[Vec3::Cord::Z] = m_sin_theta * ray.GetOrigin().GetX() + 
                            m_cos_theta * ray.GetOrigin().GetZ();

    Ray rotated_r(origin, direction, ray.GetTime());

    if (!m_object->Hit(rotated_r, ray_t, rec)) {
        return false;
    }

    Point3 p = rec.point;
    p[Vec3::Cord::X] = m_cos_theta * rec.point.GetX() + 
                    m_sin_theta * rec.point.GetZ();
    p[Vec3::Cord::Z] = -m_sin_theta * rec.point.GetX() + 
                    m_cos_theta * rec.point.GetZ();

    Vec3 normal = rec.normal;
    normal[Vec3::Cord::X] = m_cos_theta * rec.normal.GetX() + 
                            m_sin_theta * rec.normal.GetZ();
    normal[Vec3::Cord::Z] = -m_sin_theta * rec.normal.GetX() + 
                            m_cos_theta * rec.normal.GetZ();

    rec.point = p;
    rec.normal = normal;

    return true;
}

}

#endif // ROTATE_Y_HPP