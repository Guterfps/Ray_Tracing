
#include <iostream>
#include <memory>
#include <chrono>

#include "hittable_list.hpp"
#include "sphere.hpp"
#include "quad.hpp"
#include "disk.hpp"
#include "triangle.hpp"
#include "camera.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "bvh.hpp"
#include "checker_texture.hpp"
#include "image_texture.hpp"
#include "noise_texture.hpp"
#include "diffuse_light.hpp"

void BouncingSpheres();
void CheckeredSpheres();
void Earth();
void PerlinSpheres();
void Quads();
void SimpleLight();
void CornellBox();

int main(int argc, char** argv) {
    if (argc > 1) {    
        switch(std::stoi(argv[1])) {
            case 1: 
                BouncingSpheres();
                break;
            case 2:
                CheckeredSpheres();
                break;
            case 3:
                Earth();
                break;
            case 4:
                PerlinSpheres();
                break;
            case 5:
                Quads();
                break;
            case 6:
                SimpleLight();
                break;
            case 7:
                CornellBox();
                break;
            default:
            std::clog << "invalid argument(1, 2)\n";
        }
    }
    else {
        BouncingSpheres();
    }

    return 0;
}

void BouncingSpheres() {
    RayTracing::HittableList world;

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


    world = RayTracing::HittableList(
            std::vector<std::shared_ptr<RayTracing::Hittable>>{
            std::make_shared<RayTracing::BVHNode>(world)});

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

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    cam.SetBackground(RayTracing::Color(0.70, 0.80, 1.00));

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
    
}

void CheckeredSpheres() {
    RayTracing::HittableList world;

    auto checker = std::make_shared<RayTracing::CheckerTexture>(
                    0.32, RayTracing::Color(0.2, 0.3, 0.1), 
                    RayTracing::Color(0.9, 0.9, 0.9));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(0.0, -10.0, 0.0), 10.0,
        std::make_shared<RayTracing::Lambertian>(checker)));
    world.Add(std::make_shared<RayTracing::Sphere>(
        RayTracing::Point3(0.0, 10.0, 0.0), 10.0,
        std::make_shared<RayTracing::Lambertian>(checker)));

    double aspect_ratio = 16.0 / 9.0;
    double vfov = 20.0;
    double defocus_angle = 0.0;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(13, 2, 3);
    RayTracing::Point3 look_at(0, 0, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    cam.SetBackground(RayTracing::Color(0.70, 0.80, 1.00));

    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';

}

void Earth() {
    auto earth_texture = std::make_shared<RayTracing::ImageTexture>(
                        "earthmap.jpg");
    auto earth_surface = std::make_shared<RayTracing::Lambertian>(
                        earth_texture);
    auto globe = std::make_shared<RayTracing::Sphere>(
                RayTracing::Point3(0.0, 0.0, 0.0), 2.0, earth_surface);
    
    double aspect_ratio = 16.0 / 9.0;
    double vfov = 20.0;
    double defocus_angle = 0.0;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(0, 0, 12);
    RayTracing::Point3 look_at(0, 0, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    cam.SetBackground(RayTracing::Color(0.70, 0.80, 1.00));

    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(RayTracing::HittableList(
            std::vector<std::shared_ptr<RayTracing::Hittable>>{globe}), 
            true);
    
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';
}

void PerlinSpheres() {
    RayTracing::HittableList world;

    auto pertext = std::make_shared<RayTracing::NoiseTexture>(4.0);
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0, -1000, 0), 1000, 
            std::make_shared<RayTracing::Lambertian>(pertext)));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0, 2, 0), 2, 
            std::make_shared<RayTracing::Lambertian>(pertext)));

    double aspect_ratio = 16.0 / 9.0;
    double vfov = 20.0;
    double defocus_angle = 0.0;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(13, 2, 3);
    RayTracing::Point3 look_at(0, 0, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    cam.SetBackground(RayTracing::Color(0.70, 0.80, 1.00));

    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';
}

void Quads() {
    RayTracing::HittableList world;

    // Materials
    auto left_red = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(1.0, 0.2, 0.2));
    auto back_green = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.2, 1.0, 0.2));
    auto right_blue = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(1.0, 0.5, 0.0));
    auto lower_teal = std::make_shared<RayTracing::Lambertian>(
                    RayTracing::Color(0.2, 0.8, 0.8));

    // Quads
    world.Add(std::make_shared<RayTracing::Disk>(
            RayTracing::Point3(-3,-0.5, 2.5), 
            RayTracing::Vec3(0, 0,-4), 
            RayTracing::Vec3(0, 4, 0), left_red, 0.7));
    world.Add(std::make_shared<RayTracing::Quad>(
            RayTracing::Point3(-2,-2, 0), 
            RayTracing::Vec3(4, 0, 0), 
            RayTracing::Vec3(0, 4, 0), back_green));
    world.Add(std::make_shared<RayTracing::Triangle>(
            RayTracing::Point3( 3,-2, 1), 
            RayTracing::Vec3(0, 0, 4), 
            RayTracing::Vec3(0, 4, 0), right_blue));
    world.Add(std::make_shared<RayTracing::Quad>(
            RayTracing::Point3(-2, 3, 1), 
            RayTracing::Vec3(4, 0, 0), 
            RayTracing::Vec3(0, 0, 4), upper_orange));
    world.Add(std::make_shared<RayTracing::Quad>(
            RayTracing::Point3(-2,-3, 5), 
            RayTracing::Vec3(4, 0, 0), 
            RayTracing::Vec3(0, 0,-4), lower_teal));


    double aspect_ratio = 1.0;
    double vfov = 80.0;
    double defocus_angle = 0.0;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(0, 0, 9);
    RayTracing::Point3 look_at(0, 0, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);

    cam.SetBackground(RayTracing::Color(0.70, 0.80, 1.00));

    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';
}

void SimpleLight() {
    RayTracing::HittableList world;

    auto pertex = std::make_shared<RayTracing::NoiseTexture>(4.0);

    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0, -1000, 0), 1000, 
            std::make_shared<RayTracing::Lambertian>(pertex)));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0, 2, 0), 2, 
            std::make_shared<RayTracing::Lambertian>(pertex)));

    auto difflight = std::make_shared<RayTracing::DiffuseLight>(
                    RayTracing::Color(4, 4, 4));
    world.Add(std::make_shared<RayTracing::Quad>(
            RayTracing::Point3(3, 1, -2),
            RayTracing::Vec3(2, 0, 0),
            RayTracing::Vec3(0, 2, 0),
            difflight));
    world.Add(std::make_shared<RayTracing::Sphere>(
            RayTracing::Point3(0, 7, 0), 2, difflight));

    double aspect_ratio = 16.0 / 9.0;
    double vfov = 20.0;
    double defocus_angle = 0.0;
    double focus_dist = 10.0;
    uint32_t image_width = 400;
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;
    RayTracing::Point3 look_from(26, 3, 6);
    RayTracing::Point3 look_at(0, 2, 0);
    RayTracing::Vec3 vup(0, 1, 0);

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);


    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';
}

void CornellBox() {
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

    world.Add(RayTracing::Box(
                RayTracing::Point3(130, 0, 65),
                RayTracing::Point3(295, 165, 230),
                white));
    world.Add(RayTracing::Box(
                RayTracing::Point3(265, 0, 295),
                RayTracing::Point3(430, 330, 460),
                white));

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

    RayTracing::Camera cam(aspect_ratio, vfov, defocus_angle, focus_dist,
                        image_width, samples_per_pixel, max_depth,
                        look_from, look_at, vup);


    auto t1 = std::chrono::high_resolution_clock::now();
    cam.Render(world, true);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::clog << "parallel execution time: " << ms.count() << '\n';
}