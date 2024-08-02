
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>

#include "ray.hpp"
#include "vec3.hpp"
#include "interval.hpp"
#include "aabb.hpp"

namespace RayTracing {

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal);
};

inline void HitRecord::SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
    front_face = (Dot(ray.GetDirection(), outward_normal) < 0.0);
    normal = front_face ? outward_normal : (-1.0 * outward_normal);
}

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, 
                    const Interval& ray_t, 
                    HitRecord& rec) const =0;
    virtual AABB BoundingBox() const =0;
    virtual double PDFValue(const Point3& origin, const Vec3& direction) const;
    virtual Vec3 Random(const Point3& origin) const;
};


inline double Hittable::PDFValue(const Point3& origin, 
                                const Vec3& direction) const {
    (void)origin;
    (void)direction;
    
    return 0.0;
}

inline Vec3 Hittable::Random(const Point3& origin) const {
    (void)origin;
    
    return Vec3(1.0, 0.0, 0.0);
}

}

#endif // HITTABLE_HPP