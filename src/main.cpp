
#include <iostream>
#include <memory>

#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

int main(void) {
    RayTracing::HittableList world;
    double aspect_ratio = 16.0 / 9.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;

    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(0.0, 0.0, -1.0), 0.5));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0.0, -100.5, -1.0), 100));
    
    RayTracing::Camera cam(aspect_ratio, image_width, samples_per_pixel);

    cam.Render(world);

      return 0;
}
