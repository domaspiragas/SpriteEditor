#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include "toolDefinitions.h"

class PixelArray;
class Tool;

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(int width, int height, uint col = 0x00000000, QObject *parent = 0);
    ~Layer();
    Layer(const Layer& other);
    void paintEvent(QPainter& e);
    void mouseEvent(QMouseEvent* e, int x, int y, const QColor * curCol, Tool* curTool, const QColor* fillColor = 0, unsigned int brushSize = 1);

    bool IsVisible();
    int Width();
    int Height();

    QPainter* GetPainter()
    {
        return m_painter;
    }

    void Clear();
    QImage* GetImage()
    {
        return m_img;
    }
    void Resize(int,int);

signals:

public slots:
    void Hide();
    void Show();
    void SetPixel(int x, int y, uint color);
private:

    bool        m_visible;
    bool        m_update;
    uint        m_defaultCol;

    QImage*     m_img;
    QPainter*   m_painter;
    QPoint*     m_lastPoint;
    PixelArray* m_pixels;
};

#endif // LAYER_H
