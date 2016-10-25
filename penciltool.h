#ifndef PENCILTOOL_H
#define PENCILTOOL_H
#include "tool.h"

class QPoint;
class QPen;

class PencilTool : public Tool
{
public:
    PencilTool();
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0);
private:
    QPoint* m_lastPoint;
};

#endif // PENCILTOOL_H
