#include "previewwindow.h"
#include "canvas.h"
#include "frame.h"
#include <QPainter>
#include <iostream>

PreviewWindow::PreviewWindow(Canvas* canvas, QWidget *parent) : QWidget(parent)
{
    m_canvas = canvas;
    m_activeFrame = m_canvas->getActiveFrame();
    if ((unsigned int)m_activeFrame >= m_canvas->NumFrames())
        m_activeFrame = 0;
//    if (m_activeFrame < 0)
//        m_activeFrame = 0;
    m_fps = 5;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(CycleActiveFrame()));
    setMinimumSize(250,250);
}

void PreviewWindow::IncreaseFPS()
{
    if (m_fps < 30)
        m_fps++;
    emit updateFPS(m_fps);
    m_timer.stop();
    m_timer.start(1000 / m_fps);
}


void PreviewWindow::DecreaseFPS()
{
    if (m_fps > 1)
        m_fps--;
    emit updateFPS(m_fps);
    m_timer.stop();
    m_timer.start(1000 / m_fps);
}

void PreviewWindow::PreviewWindowIsDisplayed(bool display)
{
    if (display)
    {
        m_timer.start(1000 / m_fps);
        m_activeFrame = m_canvas->getActiveFrame();
        if ((unsigned int)m_activeFrame >= m_canvas->NumFrames())
            m_activeFrame = 0;
//        if (m_activeFrame < 0)
//            m_activeFrame = 0;
    }
    else
    {
        m_timer.stop();
    }
}

void PreviewWindow::CycleActiveFrame()
{
    m_activeFrame++;
    if ((unsigned int)m_activeFrame >= m_canvas->NumFrames())
        m_activeFrame = 0;
//    if (m_activeFrame < 0)
//        m_activeFrame = 0;
    repaint();
}


void PreviewWindow::paintEvent(QPaintEvent *)
{
    QPainter qp(this);
    m_canvas->GetImages().at(m_activeFrame)->paintEvent(qp);
}

void PreviewWindow::UpdateFrame(int frame)
{
    IncreaseFPS();
    DecreaseFPS();
    m_activeFrame = frame;
    this->update();
}
