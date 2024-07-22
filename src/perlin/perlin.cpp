
#include "perlin.hpp"
#include "utils.hpp"

namespace RayTracing {

double Perlin::Turb(const Point3& p, size_t depth) const {
    double accum = 0.0;
    Point3 temp_p = p;
    double weight = 1.0;

    while (depth-- > 0) {
        accum += weight * Noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return std::fabs(accum);
}

std::array<Vec3, Perlin::POINT_COUNT> Perlin::GenerateVectors() {
    std::array<Vec3, Perlin::POINT_COUNT> p;

    for (size_t i = 0; i < POINT_COUNT; ++i) {
        p[i] = UnitVector(Vec3::Random(-1.0, 1.0));
    }

    return p;
}

std::array<int, Perlin::POINT_COUNT> Perlin::PerlinGeneratePerm() {
    std::array<int, Perlin::POINT_COUNT> p;

    for (size_t i = 0; i < POINT_COUNT; ++i) {
        p[i] = i;
    }

    Permute(p, POINT_COUNT);

    return p;
}

void Perlin::Permute(std::array<int, Perlin::POINT_COUNT>& p, int n) {
    for (int i = n - 1; i > 0; --i) {
        int target = RandomInt(0, i);
        std::swap(p[i], p[target]);
    }
}

double Perlin::PerlinInterp(Vec3 c[2][2][2], 
                                double u, double v, double w) {
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0.0;

    for (uint8_t i = 0; i < 2; ++i) {
        for (uint8_t j = 0; j < 2; ++j) {
            for (uint8_t k = 0; k < 2; ++k) {
                Vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         Dot(c[i][j][k], weight_v);
            }
        }
    }

    return accum;
}

}