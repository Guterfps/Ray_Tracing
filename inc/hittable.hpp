
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "vec3.hpp"

namespace RayTracing {

struct HitRecord {
    Point3 point;
    Vec3 normal;
    double t;
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, 
                    double ray_tmin, 
                    double ray_tmax, 
                    HitRecord& rec) const =0;
};

}

#endif // HITTABLE_HPP