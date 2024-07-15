
#include "sphere.hpp"

namespace RayTracing {

bool Sphere::Hit(const Ray& ray,
            const Interval& ray_t,
            HitRecord& rec) const {
    Point3 center = m_is_moving ? SphereCenter(ray.GetTime()) : m_center;
    RayTracing::Vec3 oc = center - ray.GetOrigin();
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
    Vec3 outward_normal = (rec.point - center) / m_radius;
    rec.SetFaceNormal(ray, outward_normal);
    rec.mat = m_mat;
    auto uv = GetSphereUV(outward_normal);
    rec.u = uv.first;
    rec.v = uv.second;

    return true;
}

inline Point3 Sphere::SphereCenter(double time) const {
    return (m_center + time * m_center_vec);
}

}