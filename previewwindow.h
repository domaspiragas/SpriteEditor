#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QWidget>
#include <QTimer>

class QImage;
class Canvas;

class PreviewWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWindow(Canvas*, QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    uint getDelay() { return 1000 / m_fps; }

signals:
    void updateFPS(int);

public slots:
    void CycleActiveFrame();
    void PreviewWindowIsDisplayed(bool);
    void IncreaseFPS();
    void DecreaseFPS();
    void UpdateFrame(int frame);

private:
    Canvas* m_canvas;
    uint m_activeFrame;
    QTimer m_timer;
    int m_fps;
};

#endif // PREVIEWWINDOW_H
