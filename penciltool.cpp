#include "penciltool.h"

#include <QMouseEvent>
#include <QColor>
#include <QPainter>

PencilTool::PencilTool()
{
    m_lastPoint = 0;
}

/** Draws via pencil tool */
void PencilTool::mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillCol, unsigned int brushSize, QImage*)
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
            m_lastPoint = new QPoint();
        }
        else{
            qp->drawLine(*m_lastPoint, QPoint(x, y));
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
