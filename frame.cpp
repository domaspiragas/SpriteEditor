#include "frame.h"
#include "layer.h"
#include "tool.h"
#include "penciltool.h"
#include "bucketfill.h"
#include "linetool.h"
#include "rectangletool.h"
#include <iostream> // TODO: delete this, used for testing
#include "eraser.h"
#include "ellipsetool.h"
#include "sprayertool.h"

/** Initialize */
Frame::Frame(int w, int h, QColor backgroundColor, QObject *parent) : QObject(parent)
{
    m_layers = new std::vector<Layer*>();
    m_layers->push_back(new Layer(w, h, backgroundColor.rgba())); // The background layer
    m_layers->push_back(new Layer(w, h, QColor(255, 255, 255, 0).Rgb)); // The layer to save onto
    m_previewLayer = new Layer(w, h, 0x00ffffff);
    m_w  = w;
    m_h = h;
    m_activeLayer = 1;
    m_backgroundColor = backgroundColor;

    InitTools();
}

Frame::Frame(const Frame &other) : QObject(other.parent())
{
    m_layers = new std::vector<Layer*>();
    for(int i = 0; (unsigned int)i < other.m_layers->size(); i++)
    {
        m_layers->push_back(new Layer(*(other.m_layers)->at(i)));
    }
    m_previewLayer = new Layer(*(other.m_previewLayer));
    m_w = other.m_w;
    m_h = other.m_h;
    m_activeLayer = 1;

    InitTools();
}

void Frame::Resize(int newWidth, int newHeight)
{
    m_layers->at(0) = new Layer(newWidth, newHeight, m_backgroundColor.rgba()); // The background layer
    m_w = newWidth;
    m_h = newHeight;
    for (uint i = 0; i < m_layers->size(); i++)
    {
        m_layers->at(i)->Resize(newWidth, newHeight);
    }
//    m_activeLayer = 1;
//    InitTools();
}

void Frame::InitTools()
{
    ///Make sure to initialize any tools here!
    m_tools = new Tool*[TOOL_ACTIONS::NUM_TOOLS];
    m_tools[TOOL_ACTIONS::PENCIL_TOOL] = new PencilTool();
    m_tools[TOOL_ACTIONS::LINE_TOOL] = new LineTool(m_previewLayer);
    m_tools[TOOL_ACTIONS::RECT_TOOL] = new RectangleTool(m_previewLayer);
    m_tools[TOOL_ACTIONS::ERASER_TOOL] = new Eraser(QColor(0,0,0,0));
    m_tools[TOOL_ACTIONS::ELLIPSE_TOOL] = new EllipseTool(m_previewLayer);
    m_tools[TOOL_ACTIONS::FILL_TOOL] = 0;
    m_tools[TOOL_ACTIONS::SPRAYER_TOOL] = new SprayerTool();
}

/** cleanup */
Frame::~Frame()
{
    for(unsigned int cl = 0; cl < m_layers->size(); ++cl)
    {
        delete m_layers->at(cl);
        m_layers->at(cl) = 0;
    }
    delete m_layers;
    m_layers = 0;
    delete m_previewLayer;
    m_previewLayer = 0;

    for(int ct = 0; ct < TOOL_ACTIONS::NUM_TOOLS; ++ct)
    {
        if(m_tools[ct])
            delete m_tools[ct];
    }
    delete [] m_tools;
}

// Adds 1 because it gets passed 1 less than it should
void Frame::setActiveLayer(int layerIndex)
{
    m_activeLayer = layerIndex+1;
}

// Returns the number of editable layers, not including background/preview
int Frame::getNumLayers()
{
    return m_layers->size()-1;
}

// Add a layer to the end of the layers vector
void Frame::AddLayer()
{
     m_layers->push_back(new Layer(m_w, m_h, QColor(255, 255, 255, 0).Rgb));
}
void Frame::DeleteLayer(int index)
{
    // forced to have at least one layer
    if(m_layers->size() > 2)
    {
        m_layers->erase(m_layers->begin()+index);
    }
}

void Frame::UpdateLayerVisibility(int index, bool visible)
{
    if(visible)
    {
        m_layers->at(index)->Show();
    }
    else
    {
        m_layers->at(index)->Hide();
    }

}
bool Frame::RetrieveLayerVisibility(int index)
{
    return m_layers->at(index)->IsVisible();
}

/** Paint all layers */
void Frame::paintEvent(QPainter& e, bool showBackground) const
{
    for(unsigned int i = ((showBackground)?0:1); i < m_layers->size(); ++i)
    {
        m_layers->at(i)->paintEvent(e);
    }
    m_previewLayer->paintEvent(e);
    m_previewLayer->Clear();
}

/** handle any mouse event by passing it to the current layer */
void Frame::mouseEvent(QMouseEvent* e, int x, int y, const QColor *curCol, TOOL_ACTIONS curAction, const QColor* fillColor, unsigned int brushSize)
{
    if(curAction != FILL_TOOL)
        m_layers->at(m_activeLayer)->mouseEvent(e, x, y, curCol, m_tools[curAction], fillColor, brushSize);
    else if(!e->buttons())
    {
        BucketFill::Fill(x,y,*curCol, m_layers->at(m_activeLayer)->GetImage());
    }
}

void Frame::SetPixel(int x, int y, uint color)
{
    m_layers->at(m_activeLayer)->SetPixel(x,y,color);
}
