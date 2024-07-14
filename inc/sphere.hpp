
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace RayTracing {

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat);
    Sphere(const Point3& center1, const Point3& center2, 
        double radius, std::shared_ptr<Material> mat);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;

private:
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
m_radius(std::fmax(0, radius)), m_is_moving(false) 
{}

inline Sphere::Sphere(const Point3& center1, const Point3& center2, 
        double radius, std::shared_ptr<Material> mat) :
m_center(center1), m_center_vec(center2 - center1), m_mat(mat), 
m_radius(std::fmax(0, radius)), m_is_moving(true)
{}

}

#endif // SPHERE_HPP