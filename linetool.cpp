#include "linetool.h"
#include "layer.h"

#include <QMouseEvent>
#include <QColor>
#include <QPainter>

LineTool::LineTool(Layer* previewLayer) : PreviewTool(previewLayer)
{
    m_startPoint = 0;
    m_currentPoint = 0;
}

/** Draws via pencil tool */
void LineTool::mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* , unsigned int brushSize, QImage*)
{
    if(e->buttons())
    {
        if(!m_startPoint)
        {
            m_startPoint = new QPoint(x,y);
        }
        delete m_currentPoint;
        m_currentPoint = new QPoint(x,y);
        PreviewTool::mouseEvent(e, x, y, curCol, qp, 0, brushSize);
    }
    else
    {
        if(m_startPoint){
            SetPen(curCol, qp, brushSize);;
            qp->drawLine(*m_startPoint, *m_currentPoint);
            delete m_startPoint;
            m_startPoint = 0;
            delete m_currentPoint;
            m_currentPoint = 0;
        }
    }
}


void LineTool::DrawPreview(const QColor *previewColor, unsigned int brushSize)
{
    if(m_previewLayer && m_startPoint)
    {
        SetPen(previewColor, m_previewLayer->GetPainter(), brushSize);
        m_previewLayer->GetPainter()->drawLine(*m_startPoint, *m_currentPoint);
    }
}

