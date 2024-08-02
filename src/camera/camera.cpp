
#include <future>
#include <vector>

#include "camera.hpp"
#include "utils.hpp"
#include "hittable_pdf.hpp"
#include "cosine_pdf.hpp"
#include "mixture_pdf.hpp"

namespace RayTracing {

void Camera::Render(const Hittable& world, const Hittable& lights) {
    Initialize();

    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

    for (size_t j = 0; j < m_image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (m_image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < m_image_width; ++i) {
            Color pixel_color(0.0, 0.0, 0.0);

            for (int s_j = 0; s_j < m_sqrt_spp; ++s_j) {
                for (int s_i = 0; s_i < m_sqrt_spp; ++s_i) {
                    Ray r = GetRay(i, j, s_i, s_j);
                    pixel_color += RayColor(r, m_max_depth, world, lights);
                }
            }

            WriteColor(std::cout, 
                Color(m_pixel_samples_scale * static_cast<Vec3>(pixel_color)));
        }
    }

    std::clog << "\rDone.                 \n";
}

void Camera::Render(const Hittable& world, const Hittable& lights, bool parallel) {
    Initialize();

    std::vector<std::future<Color>> futers(m_image_height * m_image_width);

    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

    for (size_t j = 0; j < m_image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (m_image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < m_image_width; ++i) {
            futers[j * m_image_width + i] = std::async(
                [this, &world, &lights](size_t j, size_t i) {
                Color pixel_color(0.0, 0.0, 0.0);

                for (int s_j = 0; s_j < m_sqrt_spp; ++s_j) {
                    for (int s_i = 0; s_i < m_sqrt_spp; ++s_i) {
                        Ray r = GetRay(i, j, s_i, s_j);
                        pixel_color += RayColor(r, m_max_depth, world, lights);
                    }
                }

                return pixel_color;
            }, j, i);
        }
    }
    
    for (auto& f : futers) {
        Color pixel = f.get();

        WriteColor(std::cout, 
                Color(m_pixel_samples_scale * static_cast<Vec3>(pixel)));
    }

    std::clog << "\rDone.                 \n";
}


void Camera::Render(const Hittable& world, bool parallel) {
    Initialize();

    std::vector<std::future<Color>> futers(m_image_height * m_image_width);

    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

    for (size_t j = 0; j < m_image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (m_image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < m_image_width; ++i) {
            futers[j * m_image_width + i] = std::async(
                [this, &world](size_t j, size_t i) {
                Color pixel_color(0.0, 0.0, 0.0);

                for (int s_j = 0; s_j < m_sqrt_spp; ++s_j) {
                    for (int s_i = 0; s_i < m_sqrt_spp; ++s_i) {
                        Ray r = GetRay(i, j, s_i, s_j);
                        pixel_color += RayColor(r, m_max_depth, world);
                    }
                }

                return pixel_color;
            }, j, i);
        }
    }
    
    for (auto& f : futers) {
        Color pixel = f.get();

        WriteColor(std::cout, 
                Color(m_pixel_samples_scale * static_cast<Vec3>(pixel)));
    }

    std::clog << "\rDone.                 \n";
}

void Camera::Initialize() {
    m_image_height = static_cast<uint32_t>(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_sqrt_spp = static_cast<uint32_t>(std::sqrt(m_samples_per_pixel));
    m_pixel_samples_scale = 1.0 / (m_sqrt_spp * m_sqrt_spp);
    m_recip_sqrt_spp = 1.0 / m_sqrt_spp;
    
    m_center = m_look_from;
    
    double theta = DegreesToRadians(m_vfov);
    double h = std::tan(theta / 2.0);
    double viewport_hight = 2.0 * h * m_focus_dist;
    double viewport_width = viewport_hight * 
                        (static_cast<double>(m_image_width) / m_image_height);

    m_w = UnitVector(m_look_from - m_look_at);
    m_u = UnitVector(Cross(m_vup, m_w));
    m_v = Cross(m_w, m_u);
    
    Vec3 viewport_u = viewport_width * m_u;
    Vec3 viewport_v = viewport_hight * (-m_v);

    m_pixel_delta_u = viewport_u / m_image_width;
    m_pixel_delta_v = viewport_v / m_image_height;

    Vec3 viewport_upper_left = m_center - (m_focus_dist * m_w) -
                            viewport_u / 2 - viewport_v / 2;
    m_pixel00_loc = viewport_upper_left + 
                    (m_pixel_delta_u + m_pixel_delta_v) * 0.5;

    double defocus_radius = m_focus_dist * 
                        std::tan(DegreesToRadians(m_defocus_angle / 2.0));
    m_defocus_disk_u = m_u * defocus_radius;
    m_defocus_disk_v = m_v * defocus_radius;

}

Color Camera::RayColor(const Ray& ray, 
                    uint32_t depth, 
                    const Hittable& world, 
                    const Hittable& lights) const {
    if (depth == 0) {
        return Color(0.0, 0.0, 0.0);
    }
    
    HitRecord rec;

    // Interval min = 0.001 - Fixing shadow acne
    if (!world.Hit(ray, Interval(0.001, RayTracing::INF), rec)) {
        return m_background;
    }
    
    Ray scattered;
    Color attenuation;
    double pdf_value;
    Color color_from_emission = 
            rec.mat->Emitted(ray, rec, rec.u, rec.v, rec.point);
    
    if (!rec.mat->Scatter(ray, rec, attenuation, scattered, pdf_value)) {
        return color_from_emission;
    }

    auto p0 = std::make_shared<HittablePDF>(lights, rec.point);
    auto p1 = std::make_shared<CosinePDF>(rec.normal);
    MixturePDF mixed_pdf(p0, p1);

    scattered = Ray(rec.point, mixed_pdf.Generate(), ray.GetTime());
    pdf_value = mixed_pdf.Value(scattered.GetDirection());

    double scattering_pdf = rec.mat->ScatteringPDF(ray, rec, scattered);

    Color sample_color(RayColor(scattered, depth - 1, world, lights));
    Color color_from_scatter((static_cast<Vec3>(attenuation) * scattering_pdf *
                            static_cast<Vec3>(sample_color)) / pdf_value);
    
    return (color_from_emission + color_from_scatter);
}

Color Camera::RayColor(const Ray& ray, 
                    uint32_t depth, 
                    const Hittable& world) const {
    if (depth == 0) {
        return Color(0.0, 0.0, 0.0);
    }
    
    HitRecord rec;

    // Interval min = 0.001 - Fixing shadow acne
    if (!world.Hit(ray, Interval(0.001, RayTracing::INF), rec)) {
        return m_background;
    }
    
    Ray scattered;
    Color attenuation;
    double pdf_value;
    Color color_from_emission = 
            rec.mat->Emitted(ray, rec, rec.u, rec.v, rec.point);
    
    if (!rec.mat->Scatter(ray, rec, attenuation, scattered, pdf_value)) {
        return color_from_emission;
    }

    CosinePDF cosine_pdf(rec.normal);
    scattered = Ray(rec.point, cosine_pdf.Generate(), ray.GetTime());
    pdf_value = cosine_pdf.Value(scattered.GetDirection());

    double scattering_pdf = rec.mat->ScatteringPDF(ray, rec, scattered);

    Color sample_color(RayColor(scattered, depth - 1, world));
    Color color_from_scatter((static_cast<Vec3>(attenuation) * scattering_pdf *
                            static_cast<Vec3>(sample_color)) / pdf_value);
    
    return (color_from_emission + color_from_scatter);
}

// Construct a camera ray originating from the defocus disk and directed at a randomly
// sampled point around the pixel location i, j for stratified sample squre s_i, s_j.
inline Ray Camera::GetRay(int i, int j, int s_i, int s_j) const {
    Vec3 offset = SampleSquareStratified(s_i, s_j);
    Point3 pixel_sample = m_pixel00_loc 
                        + ((i + offset.GetX()) * m_pixel_delta_u)
                        + ((j + offset.GetY()) * m_pixel_delta_v);

    Point3 ray_origin = (m_defocus_angle <= 0.0) ? 
                        m_center : DefocusDiskSample();
    Vec3 ray_direction = pixel_sample - ray_origin;
    double ray_time = RandomDouble();

    return Ray(ray_origin, ray_direction, ray_time);
}

inline Point3 Camera::DefocusDiskSample() const {
    Point3 p = RandomInUnitDisk();

    return (m_center + 
        (p[Vec3::Cord::X] * m_defocus_disk_u) +
        (p[Vec3::Cord::Y] * m_defocus_disk_v));
}

inline Vec3 Camera::SampleSqure() {
    return (Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0));
}

// Returns the vector to a random point in the square sub-pixel specified by grid
// indices s_i and s_j, for an idealized unit square pixel [-.5,-.5] to [+.5,+.5].
inline Vec3 Camera::SampleSquareStratified(int s_i, int s_j) const {
    double px = ((s_i + RandomDouble()) * m_recip_sqrt_spp) - 0.5;
    double py = ((s_j + RandomDouble()) * m_recip_sqrt_spp) - 0.5;

    return Vec3(px, py, 0.0);
}

}