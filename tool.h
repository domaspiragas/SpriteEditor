#ifndef TOOL_H
#define TOOL_H
class QMouseEvent;
class QColor;
class QPainter;
class QImage;

class Tool
{
public:
    Tool();
    virtual ~Tool()
    {
    }
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1, QImage* img = 0) = 0;
    virtual void SetPen(const QColor*, QPainter*, unsigned int penSize = 1);
    virtual void SetBrush(const QColor*, QPainter*);
};

#endif // TOOL_H
