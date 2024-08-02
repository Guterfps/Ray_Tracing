
#ifndef HITTABLE_PDF_HPP
#define HITTABLE_PDF_HPP

#include "pdf.hpp"
#include "hittable_list.hpp"

namespace RayTracing {

class HittablePDF : public PDF {
public:
    HittablePDF(const Hittable& objects, const Point3& origin);

    double Value(const Vec3& direction) const override;
    Vec3 Generate() const override;

private:
    const Hittable& m_objects;
    Point3 m_origin;

};

inline HittablePDF::HittablePDF(const Hittable& objects, const Point3& origin) :
m_objects(objects), m_origin(origin)
{}

inline double HittablePDF::Value(const Vec3& direction) const {
    return m_objects.PDFValue(m_origin, direction);
}

inline Vec3 HittablePDF::Generate() const {
    return m_objects.Random(m_origin);
}

}

#endif // HITTABLE_PDF_HPP