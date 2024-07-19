
#ifndef NOISE_TEXTURE_HPP
#define NOISE_TEXTURE_HPP

#include "texture.hpp"
#include "perlin.hpp"

namespace RayTracing {

class NoiseTexture : public Texture {
public:
    NoiseTexture() = default;

    Color Value(double u, double v, const Point3& p) const override;

private:
    Perlin m_noise;

};

inline Color NoiseTexture::Value(double u, double v, const Point3& p) const {
    (void)u;
    (void)v;
    
    return Color(Vec3(1.0, 1.0, 1.0) * m_noise.Noise(p));
}

}

#endif // NOISE_TEXTURE_HPP