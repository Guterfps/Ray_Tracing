
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "color.hpp"

namespace RayTracing {

class Camera {
public:
    Camera(double aspect_ratio, 
        uint32_t image_width,
        uint32_t samples_per_pixel);

    void Render(const Hittable& world);

private:
    Point3 m_center;                    // Camera Center
    Point3 m_pixel00_loc;               // Location of pixel 0, 0
    Vec3 m_pixel_delta_u;               // Offset to pixel to the right
    Vec3 m_pixel_delta_v;               // Offset to pixel below
    double m_aspect_ratio{1.0};         // Ratio of image width over height
    double m_pixel_samples_scale;       // Color scale factor for a sum of pixel samples
    uint32_t m_image_width{100};        // Rendered image width in pixel count
    uint32_t m_image_height;            // Rendered image height
    uint32_t m_samples_per_pixel{10};   // Count of random samples for each pixel

    void Initialize();
    Color RayColor(const Ray& ray, const Hittable& world) const;
    Ray GetRay(size_t i, size_t j) const;
    static Vec3 SampleSqure();
};

inline Camera::Camera(double aspect_ratio, 
        uint32_t image_width,
        uint32_t samples_per_pixel) :
m_aspect_ratio(aspect_ratio), 
m_image_width(image_width),
m_samples_per_pixel(samples_per_pixel) {}

}

#endif // CAMERA_HPP