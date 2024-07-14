
#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

namespace RayTracing {

class Ray {
public:
    Ray();
    Ray(const Point3& origin, const Vec3& direction);
    Ray(const Point3& origin, const Vec3& direction, double time);

    Point3 GetOrigin() const;
    Vec3 GetDirection() const;
    double GetTime() const;

    Point3 At(double t) const;

private:
    Point3 m_origin;
    Vec3 m_direction;
    double m_time;
};

inline Ray::Ray(): m_origin(), m_direction()
{}

inline Ray::Ray(const Point3& origin, const Vec3& direction):
m_origin(origin), m_direction(direction), m_time(0.0)
{}

inline Ray::Ray(const Point3& origin, const Vec3& direction, double time) :
m_origin(origin), m_direction(direction), m_time(time)
{}

inline Point3 Ray::GetOrigin() const {
    return m_origin;
}

inline Vec3 Ray::GetDirection() const {
    return m_direction;
}

inline double Ray::GetTime() const {
    return m_time;
}

inline Point3 Ray::At(double t) const {
    return (m_origin + t * m_direction);
} 

}

#endif // RAY_HPP