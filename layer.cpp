#include "layer.h"
#include "pixelarray.h"
#include "tool.h"

/** initialize */
Layer::Layer(int width, int height, uint col, QObject *parent) : QObject(parent)
{
    m_img = new QImage(width, height, QImage::Format_ARGB32);
    m_img->fill(col);
    m_painter = new QPainter(m_img);
    m_visible = true;
    m_lastPoint = 0;
    m_pixels = new PixelArray(m_img);
    m_update = true;
    m_defaultCol = col;
}
Layer::Layer(const Layer &other) : QObject(other.parent())
{
    m_img = new QImage(other.m_img->copy());
    m_painter = new QPainter(m_img);
    m_visible = other.m_visible;
    m_lastPoint = 0;
    m_update = other.m_update;
    m_defaultCol = other.m_defaultCol;
}

void Layer::Resize(int newWidth,int newHeight)
{
    m_img = new QImage(m_img->copy(0, 0, newWidth, newHeight));
    m_painter = new QPainter(m_img);
//    m_lastPoint = 0;
}

/** clean up */
Layer::~Layer()
{
    delete m_painter;
    delete m_img;
}

/** hide */
void Layer::Hide()
{
    m_visible = false;
}

/** show */
void Layer::Show()
{
    m_visible = true;
}

/** draw current image to the desired painter */
void Layer::paintEvent(QPainter& e)
{
    if(m_visible){
        e.drawImage(QPoint(0,0), *m_img);
    }
}

bool Layer::IsVisible()
{
    return m_visible;
}
/** handle mouse events by painting to the qpainter for the image */
void Layer::mouseEvent(QMouseEvent* e, int x, int y, const QColor *curCol, Tool* curTool, const QColor* fillColor, unsigned int brushSize)
{
    if(curTool)
        curTool->mouseEvent(e, x, y, curCol, m_painter, fillColor, brushSize, m_img);
}

void Layer::Clear()
{
    m_img->fill(m_defaultCol);
}

void Layer::SetPixel(int x, int y, uint color)
{
    m_img->setPixel(x,y,color);
}
