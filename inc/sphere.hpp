
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <utility>

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
    static std::pair<double, double> GetSphereUV(const Point3& p);

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

inline std::pair<double, double> Sphere::GetSphereUV(const Point3& p) {
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    
    double theta = std::acos(-p.GetY());
    double phi = std::atan2(-p.GetZ(), p.GetX()) + PI;
    
    double u = phi / (2 * PI);
    double v = theta / PI;

    return std::pair(u, v);
}

}

#endif // SPHERE_HPP