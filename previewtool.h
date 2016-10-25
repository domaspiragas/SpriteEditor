#ifndef PREVIEWTOOL_H
#define PREVIEWTOOL_H

#include "tool.h"

class Layer;

class PreviewTool : public Tool
{
public:
    PreviewTool(Layer* previewPainter);
    virtual void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, QPainter* qp, const QColor* fillColor = 0, unsigned int brushSize = 1);

protected:
    virtual void DrawPreview(const QColor * previewColor, unsigned int brushSize) = 0;
    Layer* m_previewLayer = 0;
};

#endif // PREVIEWTOOL_H
