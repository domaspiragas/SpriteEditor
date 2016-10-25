#include "ellipsetool.h"

#include "layer.h"

#include <math.h>
#include <QPainter>
#include <QColor>
#include <QPoint>
#include <QMouseEvent>
#include <QPen>
#include <cstdlib> // Added to compile on Mac OS X 10 with clang

EllipseTool::EllipseTool(Layer * p)
    : PreviewTool(p)
{
    m_startPoint = 0;
    m_endPoint = 0;
}

void EllipseTool::mouseEvent(QMouseEvent *e, int x, int y, const QColor *curCol, QPainter *qp, const QColor* fillColor, unsigned int brushSize, QImage*)
{
    if(e->buttons())
    {
        delete m_endPoint;
        m_endPoint = new QPoint(x,y);
        if(!m_startPoint)
        {
            m_startPoint = new QPoint(x,y);
        }
        PreviewTool::mouseEvent(e, x, y, curCol, qp, fillColor, brushSize);
    }
    else
    {
        SetBrush(fillColor, qp);
        SetPen(curCol, qp,brushSize);
        if(m_startPoint)
            qp->drawEllipse(GetRectangle());
        delete m_startPoint;
        delete m_endPoint;
        m_startPoint = 0;
        m_endPoint = 0;
    }
}

void EllipseTool::DrawPreview(const QColor *previewColor, unsigned int brushSize)
{
    if(m_previewLayer && m_startPoint)
    {
        SetPen(previewColor, m_previewLayer->GetPainter(), brushSize);
        m_previewLayer->GetPainter()->drawEllipse(GetRectangle());
    }
}

QRect EllipseTool::GetRectangle()
{
    int width = std::abs((long)m_startPoint->x()-(long)m_endPoint->x());
    int height = std::abs((long)m_startPoint->y()-(long)m_endPoint->y());
    int x = std::min(m_startPoint->x(), m_endPoint->x());
    int y = std::min(m_startPoint->y(), m_endPoint->y());

    QRect rect(x,y,width,height);

    return rect;
}


