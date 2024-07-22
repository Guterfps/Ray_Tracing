
#ifndef NOISE_TEXTURE_HPP
#define NOISE_TEXTURE_HPP

#include "texture.hpp"
#include "perlin.hpp"

namespace RayTracing {

class NoiseTexture : public Texture {
public:
    NoiseTexture() = default;
    explicit NoiseTexture(double scale);

    Color Value(double u, double v, const Point3& p) const override;

private:
    Perlin m_noise;
    double m_scale{1.0};
};

inline NoiseTexture::NoiseTexture(double scale) : 
m_noise(), m_scale(scale) 
{}

inline Color NoiseTexture::Value(double u, double v, const Point3& p) const {
    (void)u;
    (void)v;
    
    return Color(Vec3(0.5, 0.5, 0.5) * 
        (1.0 + std::sin(m_scale * p.GetZ() + 10.0 * m_noise.Turb(p, 7))));
}

}

#endif // NOISE_TEXTURE_HPP