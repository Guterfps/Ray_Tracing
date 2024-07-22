
#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <array>
#include "vec3.hpp"

namespace RayTracing {

class Perlin {
public:
    Perlin();

    double Noise(const Point3& p) const;
    double Turb(const Point3& p, size_t depth) const;

private:
    static constexpr size_t POINT_COUNT = 256;
    std::array<Vec3, POINT_COUNT> m_rand_vecs;
    std::array<int, POINT_COUNT> m_perm_x;
    std::array<int, POINT_COUNT> m_perm_y;
    std::array<int, POINT_COUNT> m_perm_z;

    static std::array<Vec3, POINT_COUNT> GenerateVectors();
    static std::array<int, POINT_COUNT> PerlinGeneratePerm();
    static void Permute(std::array<int, POINT_COUNT>& p, int n);
    static double PerlinInterp(Vec3 c[2][2][2], 
                                double u, double v, double w);

};

inline Perlin::Perlin() : m_rand_vecs(GenerateVectors()), 
m_perm_x(PerlinGeneratePerm()),
m_perm_y(PerlinGeneratePerm()),
m_perm_z(PerlinGeneratePerm())
{}

inline double Perlin::Noise(const Point3& p) const {
    double u = p.GetX() - std::floor(p.GetX());
    double v = p.GetY() - std::floor(p.GetY());
    double w = p.GetZ() - std::floor(p.GetZ());
    
    int i = static_cast<int>(std::floor(p.GetX()));
    int j = static_cast<int>(std::floor(p.GetY()));
    int k = static_cast<int>(std::floor(p.GetZ()));

    Vec3 c[2][2][2];

    for (uint8_t di = 0; di < 2; ++di) {
        for (uint8_t dj = 0; dj < 2; ++dj) {
            for (uint8_t dk = 0; dk < 2; ++dk) {
                c[di][dj][dk] = m_rand_vecs[
                    m_perm_x[(i + di) & 255] ^
                    m_perm_y[(j + dj) & 255] ^
                    m_perm_z[(k + dk) & 255]
                ];
            }
        }
    }

    return PerlinInterp(c, u, v, w);
}

}

#endif // PERLIN_HPP