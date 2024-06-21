
#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "vec3.hpp"

namespace RayTracing {

class Color {
public:
    Color();
    Color(double r, double g, double b);

    double GetR() const;
    double GetG() const;
    double GetB() const;

private:
    Vec3 m_rgb;
};

inline Color::Color(): m_rgb() 
{}

inline Color::Color(double r, double g, double b): m_rgb(r, g,b)
{}

inline double Color::GetR() const {
    return m_rgb.GetX();
}

inline double Color::GetG() const {
    return m_rgb.GetY();
}

inline double Color::GetB() const {
    return m_rgb.GetZ();
}

// non member functions

void WriteColor(std::ostream& out, const Color& pixel_color) {
    double r = pixel_color.GetR();
    double g = pixel_color.GetG();
    double b = pixel_color.GetB();

    int rbyte = static_cast<int>(255.999 * r);
    int gbyte = static_cast<int>(255.999 * g);
    int bbyte = static_cast<int>(255.999 * b);
    
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

}

#endif // COLOR_HPP