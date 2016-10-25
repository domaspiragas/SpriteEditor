#ifndef ERASER_H
#define ERASER_H
#include "tool.h"


class QPoint;
class QPen;

class Eraser : public Tool
{

public:

    QColor* bgcolor;
    Eraser(QColor color);
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0);
private:
    QPoint* m_lastPoint;

};

#endif // ERASER_H
