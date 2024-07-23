
#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "quad.hpp"

namespace RayTracing {

class Triangle : public Quad {
public:
    Triangle(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat);

    bool IsInterior(double a, double b) const;

private:

};

inline Triangle::Triangle(const Point3& Q,
        const Vec3& u, const Vec3& v, 
        std::shared_ptr<Material> mat) :  Quad(Q, u, v, mat)
{}

inline bool Triangle::IsInterior(double a, double b) const {
    return ((a > 0) && (b > 0) && ((a + b) < 1.0));
}

}

#endif // TRIANGLE_HPP