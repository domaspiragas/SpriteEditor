#include "pixelarray.h"
#include <QImage>
#include <QColor>

/** initialize */
PixelArray::PixelArray(const QImage* i)
    : m_imgRef(i)
{
    m_w = m_imgRef->width();
    m_h = m_imgRef->height();
    m_pixels = 0;
    CreateImageArray();
}

/** cleanup */
PixelArray::~PixelArray()
{
    CleanupArray();
    m_pixels = 0;
}

/** updates the pixel array */
void PixelArray::update()
{
    if(m_w != m_imgRef->width() || m_h != m_imgRef->height())
    {
        CleanupArray();
        m_pixels = 0;
        m_w = m_imgRef->width();
        m_h = m_imgRef->height();
        CreateImageArray();
    }else{
        for(int curCol = 0; curCol < m_w; ++curCol){
            for(int curRow = 0; curRow < m_h; ++curRow ){
                QColor c( m_imgRef->pixel(curCol, curRow));
                m_pixels[curCol][curRow] = std::make_tuple((uchar)c.alpha(), (uchar)c.red(), (uchar)c.green(), (uchar)c.blue());
            }
        }
    }
}

/** creates an image array with the m_w and m_h values */
void PixelArray::CreateImageArray(){
    m_pixels = new std::tuple<uchar, uchar, uchar, uchar>*[m_w];
    for(int curCol = 0; curCol < m_w; ++curCol){
        m_pixels[curCol] = new std::tuple<uchar, uchar, uchar, uchar>[m_h];
        for(int curRow = 0; curRow < m_h; ++curRow ){
            QColor c( m_imgRef->pixel(curCol, curRow));
            m_pixels[curCol][curRow] = std::make_tuple((uchar)c.alpha(), (uchar)c.red(), (uchar)c.green(), (uchar)c.blue());
        }
    }
}

/** cleans up the pixel array */
void PixelArray::CleanupArray(){
    for(int curCol = 0; curCol < m_w; ++curCol){
        delete m_pixels[curCol];
    }
    delete [] m_pixels;
}

std::tuple<uchar, uchar, uchar, uchar> PixelArray::GetPixel(size_t x, size_t y)
{
    return m_pixels[x][y];
}
