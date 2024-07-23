
#ifndef DISK_HPP
#define DISK_HPP

#include "quad.hpp"

namespace RayTracing {

class Disk : public Quad {
public:
    Disk(const Point3& Q,
        const Vec3& u, const Vec3& v,
        std::shared_ptr<Material> mat,
        double radius);

    bool IsInterior(double a, double b) const override;

private:
    double m_radius;
};

inline Disk::Disk(const Point3& Q,
        const Vec3& u, const Vec3& v,
        std::shared_ptr<Material> mat,
        double radius) : Quad(Q, u, v, mat), m_radius(radius)
{}

inline bool Disk::IsInterior(double a, double b) const {
    return (std::sqrt(a * a + b * b) < m_radius);
}

}

#endif // DISK_HPP