#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H
#include "previewtool.h"

class QPoint;
class QRect;

class RectangleTool : public PreviewTool
{
public:
    RectangleTool(Layer* p);
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0);
protected:
    virtual void DrawPreview(const QColor * previewColor, unsigned int brushSize);
    QPoint* m_startPoint;
    QPoint* m_endPoint;
    QRect GetRectangle();
};

#endif // RECTANGLETOOL_H
