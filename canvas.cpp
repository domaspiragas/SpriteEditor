#include "canvas.h"
#include "frame.h"
#include <QPainter>
#include <iostream> // TODO: delete, for debugging

/** Initialize Values **/
Canvas::Canvas(int w, int h, QColor backgroundColor, QWidget *parent) : QWidget(parent)
{
    m_w = w;
    m_h = h;
    m_frames = new std::vector<Frame*>();
    m_backgroundColor = new QColor(backgroundColor);
    AddFrame(); // This just adds a new frame, nothing special
    m_activeFrame = 0;
    m_curCol = 0;
    m_curColFill = 0;
    SetColor(QColor(0,0,0,255));
    SetColorFill(QColor(255,255,255,255));
    m_curAction = PENCIL_TOOL;

    // Construct a QImage that is the same width and height as the frames
    m_image = QImage(w, h, QImage::Format_ARGB32);
    m_zoom = 6;
    m_zoomOutDis = true;
    setMinimumSize(m_w * m_zoom, m_h * m_zoom); // scroll bars appear on startup
}

void Canvas::setActiveFrame(int frame)
{
    m_activeFrame = frame;
}

void Canvas::Resize(int newWidth, int newHeight)
{
    m_w = newWidth;
    m_h = newHeight;
    std::vector<Frame*>* temp = m_frames;

    for (uint i = 0; i < temp->size(); ++i)
    {
        temp->at(i)->Resize(newWidth, newHeight);
    }
    m_frames = temp;
    m_image = QImage(newWidth, newHeight, QImage::Format_ARGB32);
    update();
}

int Canvas::getActiveFrame()
{
    return m_activeFrame;
}

void Canvas::DuplicateFrame(int index)
{
    m_frames->push_back(new Frame(*(m_frames->at(index))));
}
void Canvas::AddReverseAnimation()
{
    std::vector<Frame*>* temp = m_frames;
    for(int i = temp->size()-1; i >= 0; i--)
    {
        m_frames->push_back(new Frame(*(temp->at(i))));
    }
}

void Canvas::UpdateZoom(const double zoom)
{
    m_image.fill(QColor(160,160,160,255)); // Reset the background to dark
    m_zoom = zoom;
    setMinimumSize(m_w * zoom, m_h * zoom); // Causes scroll bars to appear
    if (zoom != 1)
    {
        m_image = m_image.scaled(m_w * m_zoom, m_h * m_zoom);
    }
    repaint();
}
void Canvas::ZoomIn()
{
    m_zoom += 5;
    UpdateZoom(m_zoom);
    if(m_zoom >= 26)
    {
        emit ZoomInStatus(true);
        m_zoomInDis = true;
    }
    else if(m_zoomOutDis)
    {
        emit ZoomOutStatus(false);
    }
}
void Canvas::ZoomOut()
{
     m_zoom -= 5;
     UpdateZoom(m_zoom);
     if(m_zoom <= 1)
     {
         emit ZoomOutStatus(true);
         m_zoomOutDis = true;
     }
     else if(m_zoomInDis)
     {
         emit ZoomInStatus(false);
     }
}

/** handle paint event by drawing current frame */
void Canvas::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    // Have the layers draw on the QImage instead of the canvas itself. This will allow for scaling
    QPainter qp(&m_image);
    m_frames->at(m_activeFrame)->paintEvent(qp);

    // Then scale/draw the QImage onto this canvas
    QPainter qp2(this);
    qp2.scale(m_zoom, m_zoom);
    qp2.drawImage(QPoint(0,0), m_image);
}

/** hanle mouse movement (and other mouse events) by passing it to the active frame */
void Canvas::mouseMoveEvent(QMouseEvent * e)
{
    int x = e->x();
    int y = e->y();
    x /= m_zoom;
    y /= m_zoom;

    /* The mouse event in tool's base classes should use the x and y to draw as they
       correspond to the pixel position relative to the back end QImage */
    m_frames->at(m_activeFrame)->mouseEvent(e, x, y, m_curCol, m_curAction, m_curColFill, m_curBrushSize);
    repaint();
    RedrawPreview(m_activeFrame);
}

/** cleanup */
Canvas::~Canvas()
{

    for(unsigned int cl = 0; cl < m_frames->size(); ++cl)
    {
        delete m_frames->at(cl);
        m_frames->at(cl) = 0;
    }
    delete m_frames;
    delete m_backgroundColor;
}



// Gets the number of layers from the specified frame (0-indexed)
int Canvas::getNumLayers(int frameIndexUI)
{
    return m_frames->at(frameIndexUI)->getNumLayers();
}

// Adds a layer to the specifed frame (0-indexed)
void Canvas::AddLayerSlot(int frameIndex)
{
    m_frames->at(frameIndex)->AddLayer();
}

void Canvas::DeleteLayerSlot(int frameIndex, int layerIndex)
{
    // index 0 of m_frames is the background, should never be deleted.
    m_frames->at(frameIndex)->DeleteLayer(layerIndex+1);
}

/** Used to get all of the frames as a QImage array */
const std::vector<Frame*>& Canvas::GetImages() const
{
    return *m_frames;
}

const std::vector<Frame*>& Canvas::GetImages(){
    return const_cast<const std::vector<Frame*>&>((static_cast<const Canvas&>(*this)).GetImages());
}

void Canvas::SetColor(const QColor& c)
{
    delete m_curCol;
    m_curCol = new QColor(c);
}

void Canvas::SetColorFill(const QColor& c)
{
    delete m_curColFill;
    m_curColFill = new QColor(c);
}

void Canvas::SetTool(TOOL_ACTIONS newTool){
    m_curAction = newTool;
}

void Canvas::SetBrushSize(int brushSize)
{
    m_curBrushSize = brushSize;
}

// Called by the canvas constructor and by a signal from MainWindow
void Canvas::AddFrame()
{
    m_frames->push_back(new Frame(m_w, m_h, *m_backgroundColor, this));
}

void Canvas::DeleteFrame(int index)
{
    // force to have at least one frame
    if(m_frames->size() > 1)
    {
       m_frames->erase(m_frames->begin()+index);
    }
}
void Canvas::SelectFrame(int frame)
{
    if(m_activeFrame >= 0 && (unsigned int)m_activeFrame < m_frames->size())
    {
        m_activeFrame = frame;
    }
    repaint();
}
// Sets the active layer for the currently active frame, i.e.: If the user has selected frame 2, then m_activeFrame=2
// and have selected layer 3, then the parameter is 2 (0-indexed, needs to be changed later).
void Canvas::SelectLayer(int layerIndex)
{
    m_frames->at(m_activeFrame)->setActiveLayer(layerIndex);
}
void Canvas::AcceptRepaintRequest()
{
    repaint();
}
void Canvas::UpdateLayerVisibility(QListWidgetItem* layer)
{

    m_frames->at(m_activeFrame)->UpdateLayerVisibility(layer->listWidget()->row(layer)+1,layer->checkState());
    repaint();
}
bool Canvas::LayerVisibilityStatus(int index)
{
    return m_frames->at(m_activeFrame)->RetrieveLayerVisibility(index);
}
void Canvas::SetPixel(int x, int y, uint color)
{
    m_frames->at(m_activeFrame)->SetPixel(x,y,color);
}
