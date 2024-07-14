
#include <iostream>
#include <memory>
#include <chrono>

#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"

int main(void) {
    RayTracing::HittableList world;
    
    double aspect_ratio = 16.0 / 9.0;
    double vfov = 20.0;
    double defocus_angle = 0.6;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(13, 2, 3);
    RayTracing::Point3 look_at(0, 0, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    auto ground_material = std::make_shared<RayTracing::Lambertian>(
                        RayTracing::Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0.0, -1000.0, 0.0), 1000, ground_material));
    
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = RayTracing::RandomDouble();
            RayTracing::Point3 center(a + 0.9 * RayTracing::RandomDouble(),
                                    0.2, 
                                    b + 0.9 * RayTracing::RandomDouble());
            
            RayTracing::Point3 p(4.0, 0.2, 0.0);
            if ((center - p).Length() > 0.9) {
                std::shared_ptr<RayTracing::Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = RayTracing::Color(RayTracing::Vec3::Random() *
                                RayTracing::Vec3::Random());
                    sphere_material = std::make_shared<RayTracing::Lambertian>(
                                    albedo);
                    auto center2 = center + 
                        RayTracing::Vec3(0, RayTracing::RandomDouble(0, 0.5), 0);
                    
                    world.Add(std::make_shared<RayTracing::Sphere>(
                            center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = RayTracing::Color(
                                RayTracing::Vec3::Random(0.5, 1.0));
                    auto fuzz = RayTracing::RandomDouble(0.0, 0.5);
                    sphere_material = std::make_shared<RayTracing::Metal>(
                                    albedo, fuzz);
                    world.Add(std::make_shared<RayTracing::Sphere>(
                            center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = 
                        std::make_shared<RayTracing::Dielectric>(1.5);
                    world.Add(std::make_shared<RayTracing::Sphere>(
                            center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<RayTracing::Dielectric>(1.5);
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(0.0, 1.0, 0.0), 1.0, material1));

    auto material2 = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(-4.0, 1.0, 0.0), 1.0, material2));

    auto material3 = std::make_shared<RayTracing::Metal>(
                    RayTracing::Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(4.0, 1.0, 0.0), 1.0, material3));


    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';

    // t1 = std::chrono::high_resolution_clock::now();
    // cam.Render(world);
    // t2 = std::chrono::high_resolution_clock::now();
    
    // std::chrono::duration<double, std::milli> ms2 = t2 - t1;

    // std::clog << "no parallel execution time: " << ms2.count() << '\n';
    
    return 0;
}
