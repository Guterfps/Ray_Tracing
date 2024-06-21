
#include <iostream>

#include "color.hpp"

int main(void) {

    size_t image_width = 256;
    size_t image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (size_t j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < image_width; ++i) {
            RayTracing::Color pixel_color(static_cast<double>(i) / image_width,
                                        static_cast<double>(j) / image_height, 
                                        0.0);
            RayTracing::WriteColor(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}