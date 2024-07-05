
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "vec3.hpp"
#include "interval.hpp"

namespace RayTracing {

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;

    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal);
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, 
                    const Interval& ray_t, 
                    HitRecord& rec) const =0;
};

inline void HitRecord::SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
    front_face = (Dot(ray.GetDirection(), outward_normal) < 0.0);
    normal = front_face ? outward_normal : (-1.0 * outward_normal);
}

}

#endif // HITTABLE_HPP