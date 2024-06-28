
#include <iostream>
#include <memory>

#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

int main(void) {
    RayTracing::HittableList world;
    double aspect_ratio = 16.0 / 9.0;
    size_t image_width = 400;

    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(0.0, 0.0, -1.0), 0.5));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0.0, -100.5, -1.0), 100));
    
    RayTracing::Camera cam(aspect_ratio, image_width);

    cam.Render(world);

      return 0;
}
