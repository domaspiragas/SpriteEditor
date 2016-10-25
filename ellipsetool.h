#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H
#include "previewtool.h"


class QPoint;
class QRect;

class EllipseTool : public PreviewTool
{
public:
    EllipseTool(Layer* p);
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0);
protected:
    virtual void DrawPreview(const QColor * previewColor, unsigned int brushSize);
    QPoint* m_startPoint;
    QPoint* m_endPoint;
    QRect GetRectangle();
};

#endif // ELLIPSETOOL_H
