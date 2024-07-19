
#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "texture.hpp"
#include "image_loader.hpp"

namespace RayTracing {

class ImageTexture : public Texture {
public:
    ImageTexture(const char *filename);

    Color Value(double u, double v, const Point3& p) const override;


private:
    ImageLoad m_image;

};

inline ImageTexture::ImageTexture(const char *filename) : m_image(filename)
{}

inline Color ImageTexture::Value(double u, double v, const Point3& p) const {
    (void)p;

    if (m_image.Height() <= 0) {
        return Color(0, 1, 1);
    }

    u = Interval(0.0, 1.0).Clamp(u);
    v = 1.0 - Interval(0.0, 1.0).Clamp(v);

    int i = static_cast<int>(u * m_image.Width());
    int j = static_cast<int>(v * m_image.Height());
    const unsigned char *pixel = m_image.PixelData(i, j);

    constexpr double color_scale = 1.0 / 255.0;

    return Color(color_scale * pixel[0], 
                color_scale * pixel[1], 
                color_scale * pixel[2]);
}

} 



#endif // IMAGE_TEXTURE_HPP