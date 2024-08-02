
#ifndef COSINE_PDF_HPP
#define COSINE_PDF_HPP

#include "pdf.hpp"
#include "onb.hpp"

namespace RayTracing {

class CosinePDF : public PDF {
public:
    CosinePDF(const Vec3& w);

    double Value(const Vec3& direction) const override;
    Vec3 Generate() const override;

private:
    ONB m_uvw;

};

inline CosinePDF::CosinePDF(const Vec3& w) : m_uvw(w)
{}

inline double CosinePDF::Value(const Vec3& direction) const {
    double cosine_theta = Dot(UnitVector(direction), m_uvw.W());

    return std::fmax(0.0, cosine_theta / PI);
}

inline Vec3 CosinePDF::Generate() const {
    return m_uvw.Transform(RandomCosineDirection());
}

}


#endif // COSINE_PDF_HPP