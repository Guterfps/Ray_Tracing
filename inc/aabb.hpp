
#ifndef AABB_HPP
#define AABB_HPP

#include "interval.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace RayTracing {

class AABB {
public:
    enum Axis : unsigned int {X, Y, Z, NUM_OF_AXIS};

    AABB() = default;
    AABB(const Interval& x, const Interval& y, const Interval& z);
    AABB(const Point3& a, const Point3& b);
    AABB(const AABB& box0, const AABB& box1);

    Interval AxisInterval(Axis axi) const;
    bool Hit(const Ray& ray, Interval ray_t) const;
    Axis LongestAxis() const;

    static const AABB EMPTY, UNIVERSE;

private:
    Interval m_x, m_y, m_z;

    static Interval ValidInterval(double a, double b);
    static Interval PadToMinimum(const Interval& inter);
};

inline AABB::AABB(const Interval& x, const Interval& y, const Interval& z) :
m_x(PadToMinimum(x)), m_y(PadToMinimum(y)), m_z(PadToMinimum(z))
{}

inline AABB::AABB(const Point3& a, const Point3& b) :
m_x(PadToMinimum(ValidInterval(a.GetX(), b.GetX()))),
m_y(PadToMinimum(ValidInterval(a.GetY(), b.GetY()))),
m_z(PadToMinimum(ValidInterval(a.GetZ(), b.GetZ())))
{}

inline AABB::AABB(const AABB& box0, const AABB& box1) :
m_x(box0.AxisInterval(Axis::X), box1.AxisInterval(Axis::X)),
m_y(box0.AxisInterval(Axis::Y), box1.AxisInterval(Axis::Y)),
m_z(box0.AxisInterval(Axis::Z), box1.AxisInterval(Axis::Z))
{}

inline Interval AABB::ValidInterval(double a, double b) {
    return ((a <= b) ? Interval(a, b) : Interval(b, a));
}

inline Interval AABB::AxisInterval(Axis axi) const {
    Interval inter;

    switch (axi) {
        case Axis::X : 
            inter = m_x;
            break;
        case Axis::Y : 
            inter =  m_y;
            break;
        case Axis::Z : 
            inter = m_z;
            break;
        default: 
            break;
    }

    return inter;
}

inline bool AABB::Hit(const Ray& ray, Interval ray_t) const {
    const Point3 ray_orig = ray.GetOrigin();
    const Vec3 ray_dir = ray.GetDirection();
    bool is_hit = true;

    for (unsigned int axis = 0; (axis < Axis::NUM_OF_AXIS) && is_hit; ++axis) {
        const Interval ax = AxisInterval(static_cast<Axis>(axis));
        
        Vec3::Cord cord = static_cast<Vec3::Cord>(axis); 
        const double adinv = 1.0 / ray_dir[cord];

        double t0 = (ax.GetMin() - ray_orig[cord]) * adinv;
        double t1 = (ax.GetMax() - ray_orig[cord]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.GetMin()) {
                ray_t.SetMin(t0);
            }
            if (t1 < ray_t.GetMax()) {
                ray_t.SetMax(t1);
            }
        }
        else {
            if (t1 > ray_t.GetMin()) {
                ray_t.SetMin(t1);
            }
            if (t0 < ray_t.GetMax()) {
                ray_t.SetMax(t0);
            }
        }

        if (ray_t.GetMax() <= ray_t.GetMin()) {
            is_hit = false;
        }
    }

    return is_hit;
}

inline AABB::Axis AABB::LongestAxis() const {
    return ((m_x.Size() > m_y.Size()) ? 
            ((m_x.Size() > m_z.Size()) ? Axis::X : Axis::Z) : 
            ((m_y.Size() > m_z.Size()) ? Axis::Y : Axis::Z));
}

inline Interval AABB::PadToMinimum(const Interval& inter) {
    static constexpr double delta = 0.0001;

    return ((inter.Size() < delta) ? inter.Expand(delta) : inter);
}

}

#endif // AABB_HPP