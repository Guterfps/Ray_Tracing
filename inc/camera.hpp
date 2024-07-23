
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "color.hpp"
#include "material.hpp"

namespace RayTracing {

class Camera {
public:
    Camera(double aspect_ratio = 1.0,
        double vfov = 90.0,
        double defocus_angle = 0.0,
        double focus_dist = 10.0,
        uint32_t image_width = 100,
        uint32_t samples_per_pixel = 10,
        uint32_t max_depth = 10,
        Point3 look_from = {0.0, 0.0, 0.0},
        Point3 look_at = {0.0, 0.0, -1.0},
        Vec3 vup = {0.0, 1.0, 0.0});

    void SetBackground(const Color& color);

    void Render(const Hittable& world);
    void Render(const Hittable& world, bool parallel);

private:
    Point3 m_center;                    // Camera Center
    Point3 m_pixel00_loc;               // Location of pixel 0, 0
    Vec3 m_pixel_delta_u;               // Offset to pixel to the right
    Vec3 m_pixel_delta_v;               // Offset to pixel below
    Vec3 m_u, m_v, m_w;                 // camera frame basis vectors
    Vec3 m_defocus_disk_u;              // defocus horizontal radius
    Vec3 m_defocus_disk_v;              // defocus vertical radius
    Point3 m_look_from;                 // point camera is loking from
    Point3 m_look_at;                   // point camera is loking at
    Vec3 m_vup;                         // camera relative "up" direction
    double m_aspect_ratio;              // Ratio of image width over height
    double m_pixel_samples_scale;       // Color scale factor for a sum of pixel samples
    double m_vfov;                      // vertical view angle
    double m_defocus_angle;             // Variation angle of rays through each pixel
    double m_focus_dist;                // Distance from camera lookfrom point to plane of perfect focus
    uint32_t m_image_width;             // Rendered image width in pixel count
    uint32_t m_image_height;            // Rendered image height
    uint32_t m_samples_per_pixel;       // Count of random samples for each pixel
    uint32_t m_max_depth;               // Maximum number of ray bounces into scene
    Color m_background;                 // Scene background color

    void Initialize();
    Color RayColor(const Ray& ray, uint32_t depth, const Hittable& world) const;
    Ray GetRay(size_t i, size_t j) const;
    Point3 DefocusDiskSample() const;
    static Vec3 SampleSqure();
};

inline Camera::Camera(double aspect_ratio,
        double vfov,
        double defocus_angle,
        double focus_dist,
        uint32_t image_width,
        uint32_t samples_per_pixel,
        uint32_t max_depth,
        Point3 look_from,
        Point3 look_at,
        Vec3 vup) :
m_look_from(look_from),
m_look_at(look_at),
m_vup(vup),
m_aspect_ratio(aspect_ratio),
m_vfov(vfov),
m_defocus_angle(defocus_angle),
m_focus_dist(focus_dist),
m_image_width(image_width),
m_samples_per_pixel(samples_per_pixel),
m_max_depth(max_depth),
m_background(Color(0.0, 0.0, 0.0))
 {}

inline void Camera::SetBackground(const Color& color) {
    m_background = color;
}

}

#endif // CAMERA_HPP