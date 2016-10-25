#include "eraser.h"


#include <QMouseEvent>
#include <QColor>
#include <QPainter>

QColor colors;


Eraser::Eraser(QColor color)
{
    colors = color;
    colors.setAlpha(0);

    m_lastPoint = 0;
}

/** Erases pixels */
void Eraser::mouseEvent(QMouseEvent* e, int x, int y, const QColor * , QPainter* qp, const QColor* , unsigned int brushSize, QImage* img)
{
    auto mode = QPainter::CompositionMode_SourceAtop;
    if(e->buttons() == Qt::LeftButton)
    {
        SetPen(&colors, qp, brushSize);
        qp->setBackgroundMode(Qt::BGMode::TransparentMode);
    }
    if(e->buttons())
    {
        if(!m_lastPoint)
        {
            qp->setCompositionMode(mode);
            //qp->drawPoint(x, y);
            m_lastPoint = new QPoint(x,y);
        }
        if(img)
        {
            int widthHalf = brushSize/2;
            int heightHalf = brushSize/2;
            for(int curX = 0; (unsigned int)curX < brushSize; ++curX)
            {
                for(int curY = 0; (unsigned int)curY < brushSize; ++curY)
                {
                    QPoint curPoint(x + curX - widthHalf, y + curY - heightHalf);
                    if(curPoint.x() >= 0 && curPoint.y() >= 0 && curPoint.x() < img->width() && curPoint.y() < img->height())
                        img->setPixel(curPoint,colors.rgba());
                }
            }
        }
        else
            qp->eraseRect(x, y, 1, 1);
        m_lastPoint->setX(x);
        m_lastPoint->setY(y);
    }
    else
    {
        if(m_lastPoint){
            delete m_lastPoint;
            m_lastPoint = 0;
        }
        qp->setCompositionMode(QPainter::CompositionMode_Source);
    }
}

