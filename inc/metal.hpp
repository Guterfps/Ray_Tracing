
#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"

namespace RayTracing {

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz);

    bool Scatter(const Ray& ray_in,
            const HitRecord& rec,
            Color& attenuation,
            Ray& scatterd) const override;

private:
    Color m_albedo;
    double m_fuzz;
};

inline Metal::Metal(const Color& albedo, double fuzz) : 
m_albedo(albedo), m_fuzz((fuzz < 1.0) ? fuzz : 1.0)
{}

inline bool Metal::Scatter(const Ray& ray_in,
            const HitRecord& rec,
            Color& attenuation,
            Ray& scatterd) const {
    Vec3 reflected = Reflect(ray_in.GetDirection(), rec.normal);
    
    reflected = UnitVector(reflected) + (m_fuzz * RandomUnitVector());
    scatterd = Ray(rec.point, reflected);
    attenuation = m_albedo;

    return (Dot(scatterd.GetDirection(), rec.normal) > 0.0);
}


}

#endif // METAL_HPP