
#include "perlin.hpp"
#include "utils.hpp"

namespace RayTracing {

std::array<double, Perlin::POINT_COUNT> Perlin::GenerateFloats() {
    std::array<double, Perlin::POINT_COUNT> p;

    for (size_t i = 0; i < POINT_COUNT; ++i) {
        p[i] = RandomDouble();
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

}