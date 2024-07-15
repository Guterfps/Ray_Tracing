
#ifndef CHECKER_TEXTURE_HPP
#define CHECKER_TEXTURE_HPP

#include <memory>

#include "texture.hpp"
#include "solid_color.hpp"

namespace RayTracing {

class CheckerTexture : public Texture {
public:
    CheckerTexture(double scale, 
                std::shared_ptr<Texture> even,
                std::shared_ptr<Texture> odd);
    CheckerTexture(double scale,
                const Color& c1,
                const Color& c2);

    Color Value(double u, double v, const Point3& p) const override;

private:
    double m_inv_scale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;

};

inline CheckerTexture::CheckerTexture(double scale, 
                                std::shared_ptr<Texture> even,
                                std::shared_ptr<Texture> odd) :
m_inv_scale(1.0 / scale), m_even(even), m_odd(odd)
{}

inline CheckerTexture::CheckerTexture(double scale,
                                    const Color& c1,
                                    const Color& c2) :
m_inv_scale(1.0 / scale), 
m_even(std::make_shared<SolidColor>(c1)),
m_odd(std::make_shared<SolidColor>(c2))
{}

inline Color CheckerTexture::Value(double u, double v, const Point3& p) const {
    int x_int = static_cast<int>(std::floor(m_inv_scale * p.GetX()));
    int y_int = static_cast<int>(std::floor(m_inv_scale * p.GetY()));
    int z_int = static_cast<int>(std::floor(m_inv_scale * p.GetZ()));

    bool is_odd = (x_int + y_int + z_int) & 1;

    return (is_odd ? m_odd->Value(u, v, p) : m_even->Value(u, v, p));
}

}

#endif // CHECKER_TEXTURE_HPP