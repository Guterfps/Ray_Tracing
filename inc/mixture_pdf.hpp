
#ifndef MIXTURE_PDF_HPP
#define MIXTURE_PDF_HPP

#include <memory>

#include "pdf.hpp"

namespace RayTracing {

class MixturePDF : public PDF {
public:
    MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1);

    double Value(const Vec3& direction) const override;
    Vec3 Generate() const override;

private:
    std::shared_ptr<PDF> m_pdfs[2];

};

inline MixturePDF::MixturePDF(std::shared_ptr<PDF> p0, 
                            std::shared_ptr<PDF> p1) :
m_pdfs{p0, p1}
{}

inline double MixturePDF::Value(const Vec3& direction) const {
    return (0.5 * m_pdfs[0]->Value(direction) + 
            0.5 * m_pdfs[1]->Value(direction));
}

inline Vec3 MixturePDF::Generate() const {
    return ((RandomDouble() < 0.5) ? 
            m_pdfs[0]->Generate() :
            m_pdfs[1]->Generate());
}

}

#endif // MIXTURE_PDF_HPP