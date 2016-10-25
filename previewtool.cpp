#include "previewtool.h"

#include <QMouseEvent>
#include <QColor>
#include <QPainter>
#include "layer.h"

PreviewTool::PreviewTool(Layer* previewLayer)
{
    m_previewLayer = previewLayer;
}

void PreviewTool::mouseEvent(QMouseEvent *e, int , int , const QColor *curCol, QPainter *, const QColor* , unsigned int brushSize)
{
    if(e->buttons()){
        this->DrawPreview(curCol, brushSize);
    }
}
