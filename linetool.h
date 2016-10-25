#ifndef LINETOOL_H
#define LINETOOL_H
#include "previewtool.h"

class QPoint;
class QPen;

class LineTool : public PreviewTool
{
public:
    LineTool(Layer* previewLayer);
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize =1, QImage* img = 0);
protected:
    //allow overriding, but don't let it be called externally
    virtual void DrawPreview(const QColor * previewColor, unsigned int brushSize);
private:
    QPoint* m_startPoint;
    QPoint* m_currentPoint;
};

#endif // LINETOOL_H
