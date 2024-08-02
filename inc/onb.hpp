
#ifndef ONB_HPP
#define ONB_HPP

#include "vec3.hpp"

namespace RayTracing {

class ONB {
public:
    ONB(const Vec3& n);

    Vec3 U() const;
    Vec3 V() const;
    Vec3 W() const;

    Vec3 Transform(const Vec3& v) const;

private:
    Vec3 m_axis[3];
};

inline ONB::ONB(const Vec3& n) {
    m_axis[2] = UnitVector(n);
    Vec3 a = (std::fabs(m_axis[2].GetX()) > 0.9) ? 
                Vec3(0.0, 1.0, 0.0) : 
                Vec3(1.0, 0.0, 0.0);

    m_axis[1] = UnitVector(Cross(m_axis[2], a));
    m_axis[0] = Cross(m_axis[2], m_axis[1]);
}

inline Vec3 ONB::U() const {
    return m_axis[0];
}

inline Vec3 ONB::V() const {
    return m_axis[1];
}

inline Vec3 ONB::W() const {
    return m_axis[2];
}

// Transform from basis coordinates to local space.
inline Vec3 ONB::Transform(const Vec3& v) const {
    return ((v.GetX() * m_axis[0]) +
            (v.GetY() * m_axis[1]) +
            (v.GetZ() * m_axis[2]));
}

}

#endif // ONB_HPP