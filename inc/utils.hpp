
#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <random>

namespace RayTracing {

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) {
    return (degrees * (PI / 180.0));
}

inline double RandomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;

    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    return (min + (max - min) * RandomDouble());
}

inline int RandomInt(int min, int max) {
    return static_cast<int>(RandomDouble(min, max + 1));
}

}

#endif // UTILS_HPP