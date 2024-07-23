
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "color.hpp"

namespace RayTracing {

struct HitRecord;

class Material {
public:
    virtual ~Material() =0;

    virtual bool Scatter(const Ray& ray_in,
                        const HitRecord& rec,
                        Color& attenuation,
                        Ray& scatterd) const;
    virtual Color Emitted(double u, double v, const Point3& p) const;

};

inline Material::~Material() {}

inline bool Material::Scatter(const Ray& ray_in,
                        const HitRecord& rec,
                        Color& attenuation,
                        Ray& scatterd) const {
    (void)ray_in;
    (void)rec;
    (void)attenuation;
    (void)scatterd;
    return false;
}

inline Color Material::Emitted(double u, double v, const Point3& p) const {
    (void)u; 
    (void)v;
    (void)p;

    return Color(0.0, 0.0, 0.0);
}

}

#endif // MATERIAL_HPP