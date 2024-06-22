
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "vec3.hpp"

namespace RayTracing {

struct HitRecord {
    Point3 point;
    Vec3 normal;
    double t;
    bool front_face;

    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal);
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, 
                    double ray_tmin, 
                    double ray_tmax, 
                    HitRecord& rec) const =0;
};

inline void HitRecord::SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
    front_face = (Dot(ray.GetDirection(), outward_normal) < 0.0);
    normal = front_face ? outward_normal : (-1.0 * outward_normal);
}

}

#endif // HITTABLE_HPP