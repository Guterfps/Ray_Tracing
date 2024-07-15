
#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "texture.hpp"
#include "solid_color.hpp"

namespace RayTracing {

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo);
    explicit Lambertian(std::shared_ptr<Texture> tex);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd) const override;

private:
    std::shared_ptr<Texture> m_tex;
};

inline Lambertian::Lambertian(const Color& albedo) : 
m_tex(std::make_shared<SolidColor>(albedo))
{}

inline Lambertian::Lambertian(std::shared_ptr<Texture> tex) :
m_tex(tex)
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
    attenuation = m_tex->Value(rec.u, rec.v, rec.point);

    (void)ray_in;

    return true;
}


}

#endif // LAMBERTIAN_HPP