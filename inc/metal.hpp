
#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"

namespace RayTracing {

class Metal : public Material {
public:
    Metal(const Color& albedo);

    bool Scatter(const Ray& ray_in,
            const HitRecord& rec,
            Color& attenuation,
            Ray& scatterd) const override;

private:
    Color m_albedo;
};

inline Metal::Metal(const Color& albedo) : m_albedo(albedo)
{}

inline bool Metal::Scatter(const Ray& ray_in,
            const HitRecord& rec,
            Color& attenuation,
            Ray& scatterd) const {
    Vec3 reflected = Reflect(ray_in.GetDirection(), rec.normal);
    scatterd = Ray(rec.point, reflected);
    attenuation = m_albedo;

    return true;
}


}

#endif // METAL_HPP