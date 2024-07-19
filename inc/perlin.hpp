
#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <array>
#include "vec3.hpp"

namespace RayTracing {

class Perlin {
public:
    Perlin();

    double Noise(const Point3& p) const;

private:
    static constexpr size_t POINT_COUNT = 256;
    std::array<double, POINT_COUNT> m_rand_float;
    std::array<int, POINT_COUNT> m_perm_x;
    std::array<int, POINT_COUNT> m_perm_y;
    std::array<int, POINT_COUNT> m_perm_z;

    static std::array<double, POINT_COUNT> GenerateFloats();
    static std::array<int, POINT_COUNT> PerlinGeneratePerm();
    static void Permute(std::array<int, POINT_COUNT>& p, int n);

};

inline Perlin::Perlin() : m_rand_float(GenerateFloats()), 
m_perm_x(PerlinGeneratePerm()),
m_perm_y(PerlinGeneratePerm()),
m_perm_z(PerlinGeneratePerm())
{}

inline double Perlin::Noise(const Point3& p) const {
    int i = static_cast<int>(4 * p.GetX()) & 255;
    int j = static_cast<int>(4 * p.GetY()) & 255;
    int k = static_cast<int>(4 * p.GetZ()) & 255;

    return (m_rand_float[m_perm_x[i] ^ m_perm_y[j] ^ m_perm_z[k]]);
}

}

#endif // PERLIN_HPP