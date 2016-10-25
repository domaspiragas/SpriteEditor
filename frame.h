#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <vector>
#include <QPaintEvent>
#include "toolDefinitions.h"
#include <QColor>

class Layer;
class Tool;

class Frame : public QObject
{
    Q_OBJECT
public:
    explicit Frame(int w, int h, QColor backgroundColor = QColor(0xffffffff), QObject *parent = 0);
    Frame(const Frame& other);
    ~Frame();

    void paintEvent(QPainter& e, bool showBackground = true) const;
    void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, TOOL_ACTIONS curAction, const QColor* fillColor = 0, unsigned int brushSize = 1);

    void GetDimensions(uint& width, uint& height) const
    {
        width = m_w;
        height = m_h;
    }
    void AddLayer(); // Call this to add a new layer
    int getNumLayers(); // Gets the number of editable layers, doesn't include the background or preview
    void setActiveLayer(int); // The parameter is 1 off from what you expect I think...
    void DeleteLayer(int);
    void UpdateLayerVisibility(int index, bool visible);
    bool RetrieveLayerVisibility(int index);
    void Resize(int,int);

signals:

public slots:
    void SetPixel(int x, int y, uint color);

private:
    void InitTools();

    std::vector<Layer*>* m_layers;
    Layer* m_previewLayer;
    uint m_w;
    uint m_h;
    unsigned int m_activeLayer;
    Tool** m_tools;
    QColor m_backgroundColor;
};

#endif // FRAME_H
