
#ifndef ROTATE_Y_HPP
#define ROTATE_Y_HPP

#include "hittable.hpp"
#include "utils.hpp"

namespace RayTracing {

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    AABB m_bbox;
    std::shared_ptr<Hittable> m_object;
    double m_sin_theta;
    double m_cos_theta;

    static double CalcSinTheta(double angle);
    static double CalcCosTheta(double angle);

};

inline RotateY::RotateY(std::shared_ptr<Hittable> object, double angle) :
m_bbox(object->BoundingBox()), 
m_object(object), 
m_sin_theta(CalcSinTheta(angle)), 
m_cos_theta(CalcCosTheta(angle))
{
    Point3 min(INF, INF, INF);
    Point3 max(-INF, -INF, -INF);

    for (uint8_t i = 0; i < 2; ++i) {
        for (uint8_t j = 0; j < 2; ++j) {
            for (uint8_t k = 0; k < 2; ++k) {
                Interval inter_x = m_bbox.AxisInterval(AABB::Axis::X);
                double x = i * inter_x.GetMax() + (1 - i) * inter_x.GetMin();
                Interval inter_y = m_bbox.AxisInterval(AABB::Axis::Y);
                double y = j * inter_y.GetMax() + (1 - j) * inter_y.GetMin();
                Interval inter_z = m_bbox.AxisInterval(AABB::Axis::Z);
                double z = k * inter_z.GetMax() + (1 - k) * inter_z.GetMin(); 
            
                double new_x = m_cos_theta * x + m_sin_theta * z;
                double new_z = -m_sin_theta * x + m_cos_theta * z;

                Vec3 tester(new_x, y, new_z);

                for (uint8_t c = 0; c < Vec3::Cord::NUM_OF_DIM; ++c) {
                    auto cord = static_cast<Vec3::Cord>(c);
                    min[cord] = std::fmin(min[cord], tester[cord]);
                    max[cord] = std::fmax(max[cord], tester[cord]);
                }
            }
        }
    }

    m_bbox = AABB(min, max);
}

inline bool RotateY::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    Point3 origin = ray.GetOrigin();
    Vec3 direction = ray.GetDirection();

    origin[Vec3::Cord::X] = m_cos_theta * ray.GetOrigin().GetX() - 
                            m_sin_theta * ray.GetOrigin().GetZ();
    origin[Vec3::Cord::Z] = m_sin_theta * ray.GetOrigin().GetX() + 
                            m_cos_theta * ray.GetOrigin().GetZ();
    
    direction[Vec3::Cord::X] = m_cos_theta * ray.GetDirection().GetX() - 
                            m_sin_theta * ray.GetDirection().GetZ();
    direction[Vec3::Cord::Z] = m_sin_theta * ray.GetDirection().GetX() + 
                            m_cos_theta * ray.GetDirection().GetZ();

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

inline AABB RotateY::BoundingBox() const {
    return m_bbox;
}

inline double RotateY::CalcSinTheta(double angle) {
    return std::sin(DegreesToRadians(angle));
}


inline double RotateY::CalcCosTheta(double angle) {
    return std::cos(DegreesToRadians(angle));
}

}

#endif // ROTATE_Y_HPP