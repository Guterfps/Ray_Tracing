
#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include <memory>

#include "material.hpp"
#include "texture.hpp"
#include "solid_color.hpp"
#include "sphere_pdf.hpp"

namespace RayTracing {

class Isotropic : public Material {
public:
    explicit Isotropic(const Color& albedo);
    explicit Isotropic(std::shared_ptr<Texture> tex);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                ScatterRecord& srec) const override;
    double ScatteringPDF(const Ray& r_in,
                        const HitRecord& rec,
                        const Ray& scattered) const override;

private:
    std::shared_ptr<Texture> m_tex;
};

inline Isotropic::Isotropic(const Color& albedo) :
m_tex(std::make_shared<SolidColor>(albedo))
{}

inline Isotropic::Isotropic(std::shared_ptr<Texture> tex) :
m_tex(tex)
{}

inline bool Isotropic::Scatter(const Ray& ray_in,
                                const HitRecord& rec,
                                ScatterRecord& srec) const {
    srec.attenuation = m_tex->Value(rec.u, rec.v, rec.point);
    srec.pdf_ptr = std::make_shared<SpherePDF>();
    srec.skip_pdf = false;

    return true;
}

inline double Isotropic::ScatteringPDF(const Ray& r_in,
                        const HitRecord& rec,
                        const Ray& scattered) const {
    (void)r_in;
    (void)rec;
    (void)scattered;

    return (1 / (4 * PI));
}

}

#endif // ISOTROPIC_HPP