#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QListWidgetItem>
#include <vector>
#include "toolDefinitions.h"

class Frame;
//class GIFExporter;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(int w=100, int h=100, QColor backgroundColor = QColor(0xffffffff), QWidget *parent = 0);
    ~Canvas();

    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* e)
    {
        mouseMoveEvent(e);
    }
    void mouseReleaseEvent(QMouseEvent* e)
    {
        mouseMoveEvent(e);
    }

    const std::vector<Frame*>& GetImages() const;
    const std::vector<Frame*>& GetImages();

    const QColor* GetBrushColor() const {return m_curCol;}
    const QColor* GetFillColor() const {return m_curColFill;}

    uint GetWidth() const {return m_w;}
    uint GetHeight() const {return m_h;}
    uint NumFrames() const {
        if(m_frames)
            return m_frames->size();
        return 1;
    }
    int getNumLayers(int frameIndex); // Num layers for specified frame (0 indexed)
    int getActiveFrame();
    void Resize(int,int);
    void setActiveFrame(int);

signals:
    void RedrawPreview(int);

public slots:
    void UpdateZoom(double zoom);
    void ZoomIn();
    void ZoomOut();
    void SetColor(const QColor& c);
    void SetColorFill(const QColor& c);
    void SetBrushSize(int brushSize);
    void SetTool(TOOL_ACTIONS newTool);
    void AddFrame();
    void DeleteFrame(int);//Param: index of frame to be deleted
    void SelectFrame(int);
    void SetPixel(int x, int y, uint color);
    void AddLayerSlot(int); // Param: index of frame to add layer to (0-indexed)
    void DeleteLayerSlot(int, int); // Param: index of frame to delete layer at, and layer to delete
    void SelectLayer(int); // Sets the active layer for the specified frame (0-indexed)
    void AcceptRepaintRequest();
    void UpdateLayerVisibility(QListWidgetItem*);
    bool LayerVisibilityStatus(int);
    void DuplicateFrame(int);
    void AddReverseAnimation();

signals:
    void ZoomInStatus(bool disabled);
    void ZoomOutStatus(bool disabled);
    void AddLayerToFrame(int); // Parame: Frame index (0-indexed)
    void UpdateFrameUI(int); // Call this when a new frame is added, pass index of new frame

private:
    int m_w;
    int m_h;
    int m_activeFrame;
    double m_zoom;
    bool m_zoomInDis, m_zoomOutDis;
    unsigned int m_curBrushSize = 1;

    TOOL_ACTIONS m_curAction;

    QColor* m_curCol;
    QColor* m_curColFill;
    QColor* m_backgroundColor;

    std::vector<Frame*>* m_frames;

    QImage m_image; // QPainter in paintEvent draws onto this QImage to allow zooming
};

#endif // CANVAS_H
