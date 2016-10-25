#include "sprayertool.h"

#include <QMouseEvent>
#include <QColor>
#include <QPainter>

SprayerTool::SprayerTool()
{
    m_lastPoint = 0;
}

/** Sprays around wildly */
void SprayerTool::mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillCol, unsigned int brushSize, QImage*)
{
    if(e->buttons() == Qt::LeftButton)
    {
        SetPen(curCol, qp, brushSize);
    }
    else if(e->buttons() == Qt::RightButton)
    {
        SetPen(fillCol, qp, brushSize);
    }
    if(e->buttons())
    {
        if(!m_lastPoint)
        {
            qp->drawPoint(x, y);
            qp->drawPoint(x+getRand(brushSize), y+getRand(brushSize));
            qp->drawPoint(x, y-getRand(brushSize));
            qp->drawPoint(x, y+getRand(brushSize));
            qp->drawPoint(x-getRand(brushSize), y);
            qp->drawPoint(x+getRand(brushSize), y);
            m_lastPoint = new QPoint();
        }
        else{
            qp->drawPoint(x, y);
            qp->drawPoint(x+getRand(brushSize), y+getRand(brushSize));
            qp->drawPoint(x, y-getRand(brushSize+1));
            qp->drawPoint(x, y+getRand(brushSize+1));
            qp->drawPoint(x-getRand(brushSize+2), y);
            qp->drawPoint(x+getRand(brushSize+2), y);

            qp->drawLine(*m_lastPoint, QPoint(x, y+getRand(brushSize)));
            qp->drawLine(*m_lastPoint, QPoint(x+getRand(brushSize), y));
            qp->drawLine(*m_lastPoint, QPoint(x, y-getRand(brushSize)));
            qp->drawLine(*m_lastPoint, QPoint(x-getRand(brushSize), y));


        }
        m_lastPoint->setX(x);
        m_lastPoint->setY(y);
    }
    else
    {
        if(m_lastPoint){
            delete m_lastPoint;
            m_lastPoint = 0;
        }
    }
}

int SprayerTool::getRand(int val)
{
    int value = 1;
    if (val<=1)
    {
        return qrand()% value;
    }
    else
        return qrand()% val+1;


}
