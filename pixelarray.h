#ifndef PIXELARRAY_H
#define PIXELARRAY_H

class QImage;
#include <tuple>

typedef unsigned char uchar;

class PixelArray
{
public:
    PixelArray(const QImage*);
    ~PixelArray();
    void update();

    std::tuple<uchar, uchar, uchar, uchar> GetPixel(size_t, size_t);

private:
    void CreateImageArray();
    void CleanupArray();

    const QImage* m_imgRef;
    std::tuple<uchar, uchar, uchar, uchar>** m_pixels;
    int m_w;
    int m_h;
};

#endif // PIXELARRAY_H
