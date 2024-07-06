
#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>

#include "utils.hpp"

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
    bool NearZero() const;

    static Vec3 Random();
    static Vec3 Random(double min, double max);

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
    return (Vec3(-m_e[Cord::X], -m_e[Cord::Y], -m_e[Cord::Z]));
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

inline bool Vec3::NearZero() const {
    double s = 1.0e-8;

    return ((std::fabs(m_e[Cord::X]) < s) && 
            (std::fabs(m_e[Cord::Y]) < s) &&
            (std::fabs(m_e[Cord::Z]) < s));
}

inline Vec3 Vec3::Random() {
    return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
}

inline Vec3 Vec3::Random(double min, double max) {
    return Vec3(RandomDouble(min, max), 
                RandomDouble(min, max), 
                RandomDouble(min, max));
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
    return (v * (1.0 / t));
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

inline Vec3 RandomInUnitSphere() {
    bool is_valid = false;
    Vec3 vec;

    while (!is_valid) {
        vec = Vec3::Random(-1, 1);
        is_valid = (vec.LengthSquared() < 1);
    }

    return vec;
}

inline Vec3 RandomUnitVector() {
    return UnitVector(RandomInUnitSphere());
}

inline Vec3 RandomOnHemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = RandomInUnitSphere();

    return ((Dot(on_unit_sphere, normal) > 0.0) ? 
            on_unit_sphere : -on_unit_sphere);
}

inline Vec3 Reflect(const Vec3& vec, const Vec3& normal) {
    return (vec - 2.0 * Dot(vec, normal) * normal);
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    double cos_theta = std::fmin(Dot(-uv, n), 1.0);
    Vec3 ray_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 ray_out_parallel = 
    -std::sqrt(std::fabs(1.0 - ray_out_perp.LengthSquared())) * n;

    return (ray_out_perp + ray_out_parallel);
}

} // RayTracing

#endif // VEC3_HPP