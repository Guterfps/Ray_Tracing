
#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "vec3.hpp"
#include "interval.hpp"

namespace RayTracing {

class Color {
public:
    Color();
    Color(double r, double g, double b);
    explicit Color(const Vec3& v);

    double GetR() const;
    double GetG() const;
    double GetB() const;

    explicit operator Vec3() const;
    Color& operator+=(const Color& other);

private:
    Vec3 m_rgb;
};

inline Color::Color(): m_rgb() 
{}

inline Color::Color(double r, double g, double b): m_rgb(r, g,b)
{}

inline Color::Color(const Vec3& v) : m_rgb(v) {}

inline double Color::GetR() const {
    return m_rgb.GetX();
}

inline double Color::GetG() const {
    return m_rgb.GetY();
}

inline double Color::GetB() const {
    return m_rgb.GetZ();
}

inline Color::operator Vec3() const {
    return m_rgb;
}

inline Color& Color::operator+=(const Color& other) {
    m_rgb += other.m_rgb;
    
    return *this;
}

inline Color operator+(const Color& u, const Color& v) {
    Color tmp(u);

    return (tmp += v);
}

// non member functions

inline double LinearToGamma(double linear_component) {
    return ((linear_component > 0.0) ? std::sqrt(linear_component) : 0);
}

inline void WriteColor(std::ostream& out, const Color& pixel_color) {
    double r = pixel_color.GetR();
    double g = pixel_color.GetG();
    double b = pixel_color.GetB();

    // Replace NaN components with zero
    if (r != r) {
        r = 0.0;
    }
    if (g != g) {
        g = 0.0;
    }
    if (b != b) {
        b = 0.0;
    }
    
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    static const Interval intensity(0.000, 0.999);
    
    int rbyte = static_cast<int>(256 * intensity.Clamp(r));
    int gbyte = static_cast<int>(256 * intensity.Clamp(g));
    int bbyte = static_cast<int>(256 * intensity.Clamp(b));
    
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

}

#endif // COLOR_HPP