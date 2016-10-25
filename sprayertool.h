#ifndef SPRAYERTOOL_H
#define SPRAYERTOOL_H


#include "tool.h"

class QPoint;
class QPen;

class SprayerTool : public Tool
{
public:
    SprayerTool();
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0);
private:
    QPoint* m_lastPoint;
    int getRand(int val);
};

#endif // SPRAYERTOOL_H
