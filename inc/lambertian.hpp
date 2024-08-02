
#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "texture.hpp"
#include "solid_color.hpp"
#include "onb.hpp"

namespace RayTracing {

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo);
    explicit Lambertian(std::shared_ptr<Texture> tex);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd,
                double& pdf) const override;
    double ScatteringPDF(const Ray& r_in,
                        const HitRecord& rec,
                        const Ray& scattered) const override;

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
                Ray& scatterd,
                double& pdf) const {
    ONB uvw(rec.normal);
    Vec3 scatter_direction = uvw.Transform(RandomCosineDirection());
    
    scatterd = Ray(rec.point, UnitVector(scatter_direction), ray_in.GetTime());
    attenuation = m_tex->Value(rec.u, rec.v, rec.point);
    pdf = Dot(uvw.W(), scatterd.GetDirection()) / PI;

    return true;
}

inline double Lambertian::ScatteringPDF(const Ray& r_in,
                                        const HitRecord& rec,
                                        const Ray& scattered) const {
    return (1 / (2 * PI));
}

}

#endif // LAMBERTIAN_HPP