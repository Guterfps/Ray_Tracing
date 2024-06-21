
#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

namespace RayTracing {

class Ray {
public:
    Ray();
    Ray(const Point3& origin, const Vec3& direction);

    Point3 GetOrigin() const;
    Vec3 GetDirection() const;

    Point3 At(double t) const;

private:
    Point3 m_origin;
    Vec3 m_direction;
};

inline Ray::Ray(): m_origin(), m_direction()
{}

inline Ray::Ray(const Point3& origin, const Vec3& direction):
m_origin(origin), m_direction(direction) 
{}

inline Point3 Ray::GetOrigin() const {
    return m_origin;
}

inline Vec3 Ray::GetDirection() const {
    return m_direction;
}

inline Point3 Ray::At(double t) const {
    return (m_origin + t * m_direction);
} 

}

#endif // RAY_HPP