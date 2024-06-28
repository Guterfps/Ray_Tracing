
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "color.hpp"

namespace RayTracing {

class Camera {
public:
    Camera(double aspect_ratio, size_t image_width);

    void Render(const Hittable& world);

private:
    Point3 m_center;            // Camera Center
    Point3 m_pixel00_loc;       // Location of pixel 0, 0
    Vec3 m_pixel_delta_u;       // Offset to pixel to the right
    Vec3 m_pixel_delta_v;       // Offset to pixel below
    double m_aspect_ratio{1.0}; // Ratio of image width over height
    size_t m_image_width{100};  // Rendered image width in pixel count
    size_t m_image_height;      // Rendered image height

    void Initialize();
    Color RayColor(const Ray& ray, const Hittable& world) const;
};

inline Camera::Camera(double aspect_ratio, size_t image_width) :
m_aspect_ratio(aspect_ratio), m_image_width(image_width) {}

}

#endif // CAMERA_HPP