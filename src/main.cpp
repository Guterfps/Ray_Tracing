
#include <iostream>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

RayTracing::Color RayColor(const RayTracing::Ray& ray);
double HitSphere(const RayTracing::Point3& center, 
                double radius, 
                const RayTracing::Ray& ray);

int main(void) {

    // Image

    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr size_t image_width = 400;

    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    double focal_length = 1.0;
    double viewport_hight = 2.0;
    double viewport_width = viewport_hight * 
                        (static_cast<double>(image_width) / image_height);
    RayTracing::Point3 camera_center = RayTracing::Point3(0.0, 0.0, 0.0);

    RayTracing::Vec3 viewport_u = RayTracing::Vec3(viewport_width, 0.0, 0.0);
    RayTracing::Vec3 viewport_v = RayTracing::Vec3(0.0, -viewport_hight, 0.0);

    RayTracing::Vec3 pixel_delta_u = viewport_u / image_width;
    RayTracing::Vec3 pixel_delta_v = viewport_v / image_height;

    RayTracing::Vec3 viewport_upper_left = camera_center - 
                    RayTracing::Vec3(0.0, 0.0, focal_length) -
                    viewport_u / 2 - viewport_v / 2;
    RayTracing::Vec3 pixel00_loc = viewport_upper_left + 
                    (pixel_delta_u + pixel_delta_v) / 2;


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (size_t j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < image_width; ++i) {
            RayTracing::Point3 pixel_center = pixel00_loc + 
                                (i * pixel_delta_u) + (j * pixel_delta_v);
            RayTracing::Vec3 ray_direction = pixel_center - camera_center;
            RayTracing::Ray ray(camera_center, ray_direction);

            RayTracing::Color pixel_color = RayColor(ray);
            RayTracing::WriteColor(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}

RayTracing::Color RayColor(const RayTracing::Ray& ray) {
    RayTracing::Point3 center(0.0, 0.0, -1.0);
    double t = HitSphere(center, 0.5, ray);
    if (t > 0.0) {
        RayTracing::Vec3 normal = RayTracing::UnitVector(ray.At(t) - center);
        RayTracing::Vec3 rgb = 0.5 * RayTracing::Vec3(normal.GetX() + 1, 
                                normal.GetY() + 1, normal.GetZ() + 1);
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

double HitSphere(const RayTracing::Point3& center, 
                double radius, 
                const RayTracing::Ray& ray) {
    RayTracing::Vec3 oc = center - ray.GetOrigin();
    RayTracing::Vec3 d = ray.GetDirection();
    double a = d.LengthSquared();
    double h = RayTracing::Dot(d, oc);
    double c = oc.LengthSquared() - radius * radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0) {
        return -1.0;
    } 
    else {
        return ((h - std::sqrt(discriminant)) / a);
    }
}