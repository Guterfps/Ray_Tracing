
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "color.hpp"

namespace RayTracing {

class Texture {
public:
    virtual ~Texture() = default;

    virtual Color Value(double u, double v, const Point3& p) const =0;
};

}

#endif // TEXTURE_HPP