
#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"

namespace RayTracing {

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd) const override;

private:
    Color m_albedo;
};

inline Lambertian::Lambertian(const Color& albedo) : m_albedo(albedo)
{}

inline bool Lambertian::Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd) const {
    Vec3 scatter_direction = rec.normal + RandomUnitVector();
    
    if (scatter_direction.NearZero()) {
        scatter_direction = rec.normal;
    }
    
    scatterd = Ray(rec.point, scatter_direction, ray_in.GetTime());
    attenuation = m_albedo;

    (void)ray_in;

    return true;
}


}

#endif // LAMBERTIAN_HPP