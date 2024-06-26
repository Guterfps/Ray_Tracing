
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace RayTracing {

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius);

    bool Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const override;

private:
    Point3 m_center;
    double m_radius;

};

inline Sphere::Sphere(const Point3& center, double radius) :
m_center(center), m_radius(radius) {}

bool Sphere::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    RayTracing::Vec3 oc = m_center - ray.GetOrigin();
    RayTracing::Vec3 d = ray.GetDirection();
    double a = d.LengthSquared();
    double h = RayTracing::Dot(d, oc);
    double c = oc.LengthSquared() - m_radius * m_radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0) {
        return false;
    }

    double sqrtd = std::sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (!ray_t.Surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.Surrounds(root)) {
            return false;
        }
    }

    rec.point = ray.At(root);
    rec.t = root;
    Vec3 outward_normal = (rec.point - m_center) / m_radius;
    rec.SetFaceNormal(ray, outward_normal);

    return true;
}

}

#endif // SPHERE_HPP