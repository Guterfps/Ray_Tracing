
#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "memory"

#include "material.hpp"
#include "texture.hpp"
#include "solid_color.hpp"

namespace RayTracing {

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> tex);
    DiffuseLight(const Color& emit);

    Color Emitted(const Ray& r_in, 
                const HitRecord& rec, 
                double u, double v, 
                const Point3& p) const override;

private:
    std::shared_ptr<Texture> m_tex;

};

inline DiffuseLight::DiffuseLight(std::shared_ptr<Texture> tex) : 
m_tex(tex) 
{}

inline DiffuseLight::DiffuseLight(const Color& emit) : 
m_tex(std::make_shared<SolidColor>(emit))
{}

inline Color DiffuseLight::Emitted(const Ray& r_in, 
                        const HitRecord& rec, 
                        double u, double v, 
                        const Point3& p) const {
    (void)r_in;
    
    return (rec.front_face ? m_tex->Value(u, v, p) : Color(0.0, 0.0, 0.0));
}

}

#endif // DIFFUSE_LIGHT_HPP