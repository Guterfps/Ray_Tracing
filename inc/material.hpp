
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "color.hpp"
#include "pdf.hpp"

namespace RayTracing {

struct HitRecord;

struct ScatterRecord {
    Color attenuation;
    Ray skip_pdf_ray;
    std::shared_ptr<PDF> pdf_ptr;
    bool skip_pdf;
};

class Material {
public:
    virtual ~Material() =0;

    virtual bool Scatter(const Ray& ray_in,
                        const HitRecord& rec,
                        ScatterRecord& srec) const;
    virtual Color Emitted(const Ray& r_in, 
                        const HitRecord& rec, 
                        double u, double v, 
                        const Point3& p) const;
    virtual double ScatteringPDF(const Ray& r_in, 
                                const HitRecord& rec,
                                const Ray& scattered) const;

};

inline Material::~Material() {}

inline bool Material::Scatter(const Ray& ray_in,
                        const HitRecord& rec,
                        ScatterRecord& srec) const {
    (void)ray_in;
    (void)rec;
    (void)srec;
    
    return false;
}

inline Color Material::Emitted(const Ray& r_in, 
                        const HitRecord& rec, 
                        double u, double v, 
                        const Point3& p) const {
    (void)r_in;
    (void)rec;
    (void)u;
    (void)v;
    (void)p;

    return Color(0.0, 0.0, 0.0);
}

inline double Material::ScatteringPDF(const Ray& r_in, 
                                    const HitRecord& rec,
                                    const Ray& scattered) const {
    (void)r_in;
    (void)rec;
    (void)scattered;

    return 0.0;
}

}

#endif // MATERIAL_HPP