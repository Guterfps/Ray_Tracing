
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace RayTracing {

class Sphere : public Hittable {
public:
    // Stationary Sphere
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat);
    
    // Moving Sphere
    Sphere(const Point3& center1, const Point3& center2, 
        double radius, std::shared_ptr<Material> mat);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;
    AABB BoundingBox() const override;

private:
    AABB m_bbox;
    Point3 m_center;
    Vec3 m_center_vec;
    std::shared_ptr<Material> m_mat;
    double m_radius;
    bool m_is_moving;

    Point3 SphereCenter(double time) const;

};

inline Sphere::Sphere(const Point3& center, 
                    double radius,
                    std::shared_ptr<Material> mat) :
m_center(center), m_center_vec(), m_mat(mat), 
m_radius(std::fmax(0.0, radius)), m_is_moving(false) 
{
    Vec3 rvec = Vec3(m_radius, m_radius, m_radius);
    m_bbox = AABB(m_center - rvec, m_center + rvec);
}

inline Sphere::Sphere(const Point3& center1, const Point3& center2, 
        double radius, std::shared_ptr<Material> mat) :
m_center(center1), m_center_vec(center2 - center1), m_mat(mat), 
m_radius(std::fmax(0.0, radius)), m_is_moving(true)
{
    Vec3 rvec = Vec3(m_radius, m_radius, m_radius);
    AABB box1(center1 - rvec, center1 + rvec);
    AABB box2(center2 - rvec, center2 + rvec);
    m_bbox = AABB(box1, box2);
}

inline AABB Sphere::BoundingBox() const {
    return m_bbox;
}

}

#endif // SPHERE_HPP