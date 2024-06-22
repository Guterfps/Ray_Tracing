
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
            double ray_tmin,
            double ray_tmax,
            HitRecord& rec) const override;

private:
    Point3 m_center;
    double m_radius;

    static bool NotInRange(double num, double min, double max);

};

inline Sphere::Sphere(const Point3& center, double radius) :
m_center(center), m_radius(radius) {}

bool Sphere::Hit(const Ray& ray,
            double ray_tmin,
            double ray_tmax,
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
    if (NotInRange(root, ray_tmin, ray_tmax)) {
        root = (h + sqrtd) / a;
        if (NotInRange(root, ray_tmin, ray_tmax)) {
            return false;
        }
    }

    rec.point = ray.At(root);
    rec.normal = (rec.point - m_center) / m_radius;
    rec.t = root;

    return true;
}

inline static bool NotInRange(double num, double min, double max) {
    return ((num <= min) || (num >= max));
}

}

#endif // SPHERE_HPP