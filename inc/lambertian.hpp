
#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "texture.hpp"
#include "solid_color.hpp"
#include "cosine_pdf.hpp"

namespace RayTracing {

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo);
    explicit Lambertian(std::shared_ptr<Texture> tex);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                ScatterRecord& srec) const override;
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
                ScatterRecord& srec) const {
    srec.attenuation = m_tex->Value(rec.u, rec.v, rec.point);
    srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
    srec.skip_pdf = false;

    return true;
}

inline double Lambertian::ScatteringPDF(const Ray& r_in,
                                        const HitRecord& rec,
                                        const Ray& scattered) const {
    double cos_theta = Dot(rec.normal, UnitVector(scattered.GetDirection()));
    
    return ((cos_theta < 0.0) ? 0.0 : (cos_theta / PI));
}

}

#endif // LAMBERTIAN_HPP