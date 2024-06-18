
#include <iostream>

int main(void) {

    size_t image_width = 256;
    size_t image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (size_t j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << 
        (image_height - j) << ' ' << std::flush;
        
        for (size_t i = 0; i < image_width; ++i) {
            double r = static_cast<double>(i) / (image_width - 1);
            double g = static_cast<double>(j) / (image_height - 1);
            double b = 0.0;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}