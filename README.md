# Ray Tracing Renderer

This project is a ray tracing renderer written in C++ and built using the Zig build tool. It aims to provide a simple yet powerful implementation of a ray tracing renderer. 
(The project is a work in progress)

## Features

- Basic ray tracing capabilities
- Support for Spheres, Boxes and Quadrilaterals
- Diffuse, reflective, and refractive materials
- Anti-aliasing
- Motion Blur
- Bounding Volume Hierarchies
- Texture Mapping
- Loading .jpg textures
- Perlin Noise
- Lights
- Volumes

## Getting Started

### Prerequisites

Make sure you have the following installed:

- [Zig](https://ziglang.org/download/)

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/Guterfps/Ray_Tracing.git
    cd Ray_Tracing
    ```

2. Build the project using Zig:
    ```sh
    zig build
    ```

### Usage

To run the ray tracing renderer, use the following command:
```sh
zig build run > output.ppm
```
### Examples

Code example for a Cornell Box:
```cpp
    #include <memory>

    #include "hittable_list.hpp"
    #include "lambertian.hpp"
    #include "diffuse_light.hpp"
    #include "quad.hpp"
    #include "rotate_y.hpp"
    #include "translate.hpp"
    #include "camera.hpp"

    int main(void) {
        RayTracing::HittableList world;

        auto red = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.65, 0.05, 0.05));
        auto white = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.73, 0.73, 0.73));
        auto green = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.12, 0.45, 0.15));
        auto light = std::make_shared<RayTracing::DiffuseLight>(
                    RayTracing::Color(15, 15, 15));

        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(555, 0, 0),
                RayTracing::Vec3(0, 555, 0),
                RayTracing::Vec3(0, 0, 555),
                green));
        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(0, 0, 0),
                RayTracing::Vec3(0, 555, 0),
                RayTracing::Vec3(0, 0, 555),
                red));
        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(343, 554, 332),
                RayTracing::Vec3(-130, 0, 0),
                RayTracing::Vec3(0, 0, -105),
                light));
        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(0, 0, 0),
                RayTracing::Vec3(555, 0, 0),
                RayTracing::Vec3(0, 0, 555),
                white));
        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(555, 555, 555),
                RayTracing::Vec3(-555, 0, 0),
                RayTracing::Vec3(0, 0, -555),
                white));
        world.Add(std::make_shared<RayTracing::Quad>(
                RayTracing::Point3(0, 0, 555),
                RayTracing::Vec3(555, 0, 0),
                RayTracing::Vec3(0, 555, 0),
                white));

        std::shared_ptr<RayTracing::Hittable> box1 = RayTracing::Box(
                    RayTracing::Point3(0, 0, 0),
                    RayTracing::Point3(165, 330, 165),
                    white);
        box1 = std::make_shared<RayTracing::RotateY>(box1, 15);
        box1 = std::make_shared<RayTracing::Translate>(box1, 
                                RayTracing::Vec3(265, 0, 295));

        std::shared_ptr<RayTracing::Hittable> box2 = RayTracing::Box(
                    RayTracing::Point3(0, 0, 0),
                    RayTracing::Point3(165, 165, 165),
                    white);
        box2 = std::make_shared<RayTracing::RotateY>(box2, -18);
        box2 = std::make_shared<RayTracing::Translate>(box2, 
                                RayTracing::Vec3(130, 0, 65));

        world.Add(box1);
        world.Add(box2);

        double aspect_ratio = 1.0;
        double vfov = 40.0;
        double defocus_angle = 0.0;
        double focus_dist = 10.0;
        uint32_t image_width = 600;
        uint32_t samples_per_pixel = 200;
        uint32_t max_depth = 50;
        RayTracing::Point3 look_from(278, 278, -800);
        RayTracing::Point3 look_at(278, 278, 0);
        RayTracing::Vec3 vup(0, 1, 0);

        RayTracing::Camera cam(aspect_ratio, vfov, 
                                defocus_angle,focus_dist,
                                image_width, samples_per_pixel, max_depth,
                                look_from, look_at, vup);

        cam.Render(world, true);

        return 0;
    }
```

You can run the examples from the src/main.cpp file like this:
```sh
    zig build run -- <example number(1 - 9)> > output.ppm
```

Image showcasing some of the fetures:
![alt text](/out/image.ppm)


## Project Structure

- `src/`: Contains the main.cpp file and source files for the renderer.
- `inc/`: Contains header files.
- `out/`: Directory for output images.
- `images/`: Sample images used for textures.
- `build.zig`: Zig build configuration file.

## Dependencies

The only external dependency is `stb_image.h` for loading .jpg files. This is included in the repository, so you don't need to install it separately.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

This project is based on the "Ray Tracing in One Weekend" book series. Special thanks to the authors and contributors of the series for their invaluable resources.