
#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>

namespace RayTracing {

class Vec3 {
public:
    enum Cord: unsigned int {X = 0, Y, Z, NUM_OF_DIM};

    Vec3();
    Vec3(double e0, double e1, double e2);

    double GetX() const;
    double GetY() const;
    double GetZ() const;

    Vec3 operator-() const;
    double operator[](Cord i) const;
    double& operator[](Cord i);

    Vec3& operator+=(const Vec3& v);
    Vec3& operator*=(double t);
    Vec3& operator/=(double t);

    double Length() const;
    double LengthSquared() const;


private:
    double m_e[NUM_OF_DIM];
};

inline Vec3::Vec3() : m_e{0.0, 0.0, 0.0} {}

inline Vec3::Vec3(double e0, double e1, double e2) : m_e{e0, e1, e2} 
{}

inline double Vec3::GetX() const {
    return m_e[Cord::X];
}

inline double Vec3::GetY() const {
    return m_e[Cord::Y];
}

inline double Vec3::GetZ() const {
    return m_e[Cord::Z];
}

inline Vec3 Vec3::operator-() const {
    return (Vec3(-m_e[0], -m_e[1], -m_e[2]));
}

inline double Vec3::operator[](Cord i) const {
    return m_e[i];
}

inline double& Vec3::operator[](Cord i) {
    return m_e[i];
}

inline Vec3& Vec3::operator+=(const Vec3& v) {
    m_e[Cord::X] += v.m_e[Cord::X];
    m_e[Cord::Y] += v.m_e[Cord::Y];
    m_e[Cord::Z] += v.m_e[Cord::Z];

    return *this;
}

inline Vec3& Vec3::operator*=(double t) {
    m_e[Cord::X] *= t;
    m_e[Cord::Y] *= t;
    m_e[Cord::Z] *= t;

    return *this;
}

inline Vec3& Vec3::operator/=(double t) {
    return (*this *= (1 / t));
}

inline double Vec3::Length() const {
    return (std::sqrt(LengthSquared()));
}

inline double Vec3::LengthSquared() const {
    return (m_e[Cord::X] * m_e[Cord::X] +
            m_e[Cord::Y] * m_e[Cord::Y] +
            m_e[Cord::Z] * m_e[Cord::Z]);
}

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

// non member functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return (out << v[Vec3::Cord::X] << ' ' 
            << v[Vec3::Cord::Y] << ' ' 
            << v[Vec3::Cord::Z]);
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    Vec3 tmp(u);

    return (tmp += v);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return (u + (-v));
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.GetX() * v.GetX(), 
                u.GetY() * v.GetY(), 
                u.GetZ() * v.GetZ());
}

inline Vec3 operator*(double t, const Vec3& v) {
    Vec3 tmp(v);

    return (tmp *= t);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return (t * v);
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (v * (1 / t));
}

inline double Dot(const Vec3& u, const Vec3& v) {
    return (u.GetX() * v.GetX() + 
            u.GetY() * v.GetY() +
            u.GetZ() * v.GetZ());
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.GetY() * v.GetZ() - u.GetZ() * v.GetY(),
                u.GetZ() * v.GetX() - u.GetX() * v.GetZ(),
                u.GetX() * v.GetY() - u.GetY() * v.GetX());
}

inline Vec3 UnitVector(const Vec3& v) {
    return (v / v.Length());
}

} // RayTracing

#endif // VEC3_HPP