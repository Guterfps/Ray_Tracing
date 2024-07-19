
#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <memory>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_FAILURE_USERMSG
#include "stb_image/stb_image.h"

namespace RayTracing {

class ImageLoad {
public:
    ImageLoad() = default;
    ImageLoad(const char *image_filename);
    
    bool Load(const std::string& filename);
    int Width() const;
    int Height() const;
    const unsigned char *PixelData(int x, int y) const;

private:
    const int m_bytes_per_pixel{3};
    std::unique_ptr<float> m_fdata{nullptr};
    std::unique_ptr<unsigned char[]> m_bdata{nullptr};
    int m_image_width{0};
    int m_image_height{0};
    int m_bytes_per_scanline{0};

    static int Clamp(int x, int low, int high);
    static unsigned char FloatToByte(float value);
    void ConvertToBytes();

};

inline ImageLoad::ImageLoad(const char *image_filename) {
    std::string file_name(image_filename);
    const char *image_dir = getenv("IMAGES");

    if ((image_dir != nullptr) && Load(image_dir + '/' + file_name)) {} 
    else if (Load(file_name)) {}
    else if (Load("images/" + file_name)) {}
    else if (Load("../images/" + file_name)) {}
    else {
        std::cerr << "ERROR: could not load image file '" 
                    << image_filename << "'.\n";
    }
}

inline bool ImageLoad::Load(const std::string& filename) {
    int n = m_bytes_per_pixel;
    m_fdata = std::unique_ptr<float>(
                stbi_loadf(filename.c_str(), &m_image_width, &m_image_height, 
                            &n, m_bytes_per_pixel));

    if (m_fdata.get() == nullptr) {
        return false;
    }

    m_bytes_per_scanline = m_image_width * m_bytes_per_pixel;
    ConvertToBytes();

    return true;
}

inline int ImageLoad::Width() const {
    return ((m_fdata.get() == nullptr) ? 0 : m_image_width);
}

inline int ImageLoad::Height() const {
    return ((m_fdata.get() == nullptr) ? 0 : m_image_height);
}

inline const unsigned char *ImageLoad::PixelData(int x, int y) const {
    // return the address of three RGB bytes of the pixel at (x,y).
    // if there is no image data, rturns magenta.

    static unsigned char magenta[3] = {255, 0, 255};

    if (m_bdata.get() == nullptr) {
        return magenta;
    }

    x = Clamp(x, 0, m_image_width);
    y = Clamp(y, 0, m_image_height);

    return (m_bdata.get() + y * m_bytes_per_scanline + x * m_bytes_per_pixel);
}

inline int ImageLoad::Clamp(int x, int low, int high) {
    // return the value clamped to the range [low, high)

    return ((x < low) ? low : ((x < high) ? x : high - 1));
}

inline unsigned char ImageLoad::FloatToByte(float value) {
    return ((value <= 0.0) ? 0 : 
            ((value >= 1.0) ? 255 : 
            static_cast<unsigned char>(256.0 * value)));
}

inline void ImageLoad::ConvertToBytes() {
    int total_bytes = m_image_width * m_image_height * m_bytes_per_pixel;
    m_bdata = std::unique_ptr<unsigned char[]>(new unsigned char[total_bytes]);

    unsigned char *bptr = m_bdata.get();
    float *fptr = m_fdata.get();

    for (int i = 0; i < total_bytes; ++i, ++bptr, ++fptr) {
        *bptr = FloatToByte(*fptr);
    }
}

}

#endif // IMAGE_LOADER_HPP