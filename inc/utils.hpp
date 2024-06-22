
#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>

namespace RayTracing {

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) {
    return (degrees * (PI / 180.0));
}

}


#endif // UTILS_HPP