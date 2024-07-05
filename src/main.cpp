
#include <iostream>
#include <memory>

#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"

int main(void) {
    RayTracing::HittableList world;
    double aspect_ratio = 16.0 / 9.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;

    auto material_ground = std::make_shared<RayTracing::Lambertian>
                        (RayTracing::Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<RayTracing::Lambertian>(
                        RayTracing::Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<RayTracing::Metal>(
                        RayTracing::Color(0.8, 0.8, 0.8));
    auto material_right  = std::make_shared<RayTracing::Metal>(
                        RayTracing::Color(0.8, 0.6, 0.2));

    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3( 1.0,    0.0, -1.0),   0.5, material_right));


    RayTracing::Camera cam(aspect_ratio, image_width, 
                            samples_per_pixel, max_depth);

    cam.Render(world);

      return 0;
}
