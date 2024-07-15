
#ifndef SOLID_COLOR_HPP
#define SOLID_COLOR_HPP

#include "texture.hpp"

namespace RayTracing {

class SolidColor : public Texture {
public:
    explicit SolidColor(const Color& albedo);
    SolidColor(double r, double g, double b);

    Color Value(double u, double v, const Point3& p) const override;

private:
    Color m_albedo;

};

inline SolidColor::SolidColor(const Color& albedo) : m_albedo(albedo)
{}

inline SolidColor::SolidColor(double r, double g, double b) :
SolidColor(Color(r, g, b))
{}

inline Color SolidColor::Value(double u, double v, const Point3& p) const {
    return m_albedo;
}

}

#endif // SOLID_COLOR_HPP