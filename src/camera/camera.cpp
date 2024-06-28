
#include "camera.hpp"

namespace RayTracing {

void Camera::Render(const Hittable& world) {
    Initialize();

    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

    for (size_t j = 0; j < m_image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (m_image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < m_image_width; ++i) {
            Point3 pixel_center = m_pixel00_loc + 
                                (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
            Vec3 ray_direction = pixel_center - m_center;
            Ray ray(m_center, ray_direction);

            Color pixel_color = RayColor(ray, world);
            WriteColor(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}

void Camera::Initialize() {
    m_image_height = static_cast<size_t>(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_center = Point3(0.0, 0.0, 0.0);
    
    double focal_length = 1.0;
    double viewport_hight = 2.0;
    double viewport_width = viewport_hight * 
                        (static_cast<double>(m_image_width) / m_image_height);

    Vec3 viewport_u = Vec3(viewport_width, 0.0, 0.0);
    Vec3 viewport_v = Vec3(0.0, -viewport_hight, 0.0);

    m_pixel_delta_u = viewport_u / m_image_width;
    m_pixel_delta_v = viewport_v / m_image_height;

    Vec3 viewport_upper_left = m_center - Vec3(0.0, 0.0, focal_length) -
                                viewport_u / 2 - viewport_v / 2;
    m_pixel00_loc = viewport_upper_left + 
                    (m_pixel_delta_u + m_pixel_delta_v) / 2;

}

Color Camera::RayColor(const Ray& ray, const Hittable& world) const {
    RayTracing::HitRecord rec;

    if (world.Hit(ray, RayTracing::Interval(0.0, RayTracing::INF), rec)) {
        RayTracing::Vec3 rgb = 0.5 * 
                        (rec.normal + RayTracing::Vec3(1.0, 1.0, 1.0));
        
        return RayTracing::Color(rgb.GetX(), rgb.GetY(), rgb.GetZ());
    }
    
    // linear interpolation
    
    RayTracing::Vec3 unit_direction = RayTracing::UnitVector(ray.GetDirection());
    double a = 0.5 * (unit_direction.GetY() + 1.0);
    RayTracing::Vec3 v = (1.0 - a) * 
            static_cast<RayTracing::Vec3>(RayTracing::Color(1.0, 1.0, 1.0)) + 
            a * static_cast<RayTracing::Vec3>(RayTracing::Color(0.5, 0.7, 1.0)); 
    
    return RayTracing::Color(v.GetX(), v.GetY(), v.GetZ());
}

}