
#ifndef PDF_HPP
#define PDF_HPP

#include "vec3.hpp"

namespace RayTracing {

class PDF {
public:
    virtual ~PDF();

    virtual double Value(const Vec3& direction) const =0;
    virtual Vec3 Generate() const =0;

};

inline PDF::~PDF() {}

}

#endif // PDF_HPP