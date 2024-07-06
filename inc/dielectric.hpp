
#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "material.hpp"

namespace RayTracing {

class Dielectric : public Material {
public:
    explicit Dielectric(double refraction_index);

    bool Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd) const override;

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double m_refraction_index;

    static double Reflectance(double cosine, double refraction_index);

};

inline Dielectric::Dielectric(double refraction_index) : 
m_refraction_index(refraction_index) 
{}

inline bool Dielectric::Scatter(const Ray& ray_in,
                const HitRecord& rec,
                Color& attenuation,
                Ray& scatterd) const {
    attenuation = Color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? 
                (1.0 / m_refraction_index) : m_refraction_index;
    
    Vec3 unit_direction = UnitVector(ray_in.GetDirection());
    double cos_theta = std::fmin(Dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = (ri * sin_theta) > 1.0;

    Vec3 direction;
    if (cannot_refract || (Reflectance(cos_theta, ri) > RandomDouble())) {
        direction = Reflect(unit_direction, rec.normal);
    }
    else {
        direction = Refract(unit_direction, rec.normal, ri);
    }

    scatterd = Ray(rec.point, direction);

    return true;
}

// Use Schlick's approximation for reflectance.
inline double Dielectric::Reflectance(double cosine, double refraction_index) {
    double r0 = (1.0 - refraction_index) / (1 + refraction_index);
    r0 *= r0;

    return (r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5));
}

}

#endif // DIELECTRIC_HPP