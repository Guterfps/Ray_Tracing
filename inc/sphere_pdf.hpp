
#ifndef SPHERE_PDF_HPP
#define SPHERE_PDF_HPP

#include "pdf.hpp"

namespace RayTracing {

class SpherePDF : public PDF {
public:
    SpherePDF();

    double Value(const Vec3& direction) const override;
    Vec3 Generate() const override;

};

inline SpherePDF::SpherePDF() 
{}

inline double SpherePDF::Value(const Vec3& direction) const {
    return (1 / (4 * PI));
}

inline Vec3 SpherePDF::Generate() const {
    return RandomUnitVector();
}

}

#endif // SPHERE_PDF_HPP