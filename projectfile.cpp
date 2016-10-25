#include "projectfile.h"
#include "canvas.h"
#include <fstream>
#include <sstream>
#include <QColor>
#include <QPainter>
#include "frame.h"
#include <string>

ProjectFile::ProjectFile(const char * fileName)
    : m_height(0),
      m_width(0),
      m_numFrames(0),
      m_colorGrid(0)
{
    std::ifstream infile(fileName);
    std::string line;
    int stage = 0;
    unsigned int curRow = 0;
    unsigned int curFrame = 0;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        switch(stage)
        {
        case 0:
            if(!(iss >> m_height >> m_width))
            {
                throw new std::exception();
            }
            ++stage;
            break;
        case 1:
            if(!(iss >> m_numFrames))
            {
                throw new std::exception();
            }
            ++stage;
            PopulateColorGrid();
            break;
        default:
            {
                for(unsigned int curCol = 0; curCol < m_width; ++curCol)
                {
                    uint r, g, b, a;
                    if(!(iss >> r >> g >> b >> a))
                        throw new std::exception();
                    m_colorGrid[curFrame][curRow][curCol].m_colRGBA.a = a;
                    m_colorGrid[curFrame][curRow][curCol].m_colRGBA.r = r;
                    m_colorGrid[curFrame][curRow][curCol].m_colRGBA.g = g;
                    m_colorGrid[curFrame][curRow][curCol].m_colRGBA.b = b;
                }
                ++curRow;
                if(curRow >= m_height)
                {
                    curRow = 0;
                    curFrame++;
                }
            }
        }
    }
}

ProjectFile::ProjectFile(const Canvas * cnvs)
    : m_height(cnvs->GetHeight()),
      m_width(cnvs->GetWidth()),
      m_numFrames(cnvs->NumFrames()),
      m_colorGrid(0)
{
    PopulateColorGrid();
    GrabContentFromCanvas(cnvs);
}

ProjectFile::~ProjectFile()
{
    DestroyColorGrid();
}

void ProjectFile::DestroyColorGrid()
{
    if(m_colorGrid)
    {
        for(uint curFrame = 0; curFrame < m_numFrames; ++curFrame)
        {
            for(uint curRow = 0; curRow < m_height; ++curRow)
            {
                delete [] m_colorGrid[curFrame][curRow];
                m_colorGrid[curFrame][curRow] = 0;
            }
            delete [] m_colorGrid[curFrame];
            m_colorGrid[curFrame] = 0;
        }
        delete [] m_colorGrid;
        m_colorGrid = 0;
    }
}

void ProjectFile::PopulateColorGrid()
{
    DestroyColorGrid();
    m_colorGrid = new Color**[m_numFrames];
    for(uint curFrame = 0; curFrame < m_numFrames; ++curFrame)
    {
        m_colorGrid[curFrame] = new Color*[m_height];
        for(uint curRow = 0; curRow < m_height; ++curRow)
        {
            m_colorGrid[curFrame][curRow] = new Color[m_width];
            for(uint curCol = 0; curCol < m_width; ++curCol)
                m_colorGrid[curFrame][curRow][curCol].m_col = 0;
        }
    }
}

bool ProjectFile::Save(const char* fileName)
{
    std::ofstream projectFileOutput;
    projectFileOutput.open(fileName);

    if(!projectFileOutput.is_open())
        return false;

    projectFileOutput << std::to_string(m_height) << " " << std::to_string(m_width) << "\n" << std::to_string(m_numFrames) << "\n";

    for(uint curFrame = 0; curFrame < m_numFrames; ++curFrame)
    {
        for(uint curRow = 0; curRow < m_height; ++curRow)
        {
            for(uint curCol = 0; curCol < m_width; ++curCol)
            {
                projectFileOutput << std::to_string(m_colorGrid[curFrame][curRow][curCol].m_colRGBA.r) << " ";
                projectFileOutput << std::to_string(m_colorGrid[curFrame][curRow][curCol].m_colRGBA.g) << " ";
                projectFileOutput << std::to_string(m_colorGrid[curFrame][curRow][curCol].m_colRGBA.b) << " ";
                projectFileOutput << std::to_string(m_colorGrid[curFrame][curRow][curCol].m_colRGBA.a) << " ";
            }
            projectFileOutput << "\n";
        }
    }

    projectFileOutput.close();

    return true;
}

void ProjectFile::GrabContentFromCanvas(const Canvas * cnvs)
{
    const std::vector<Frame*>& frames = cnvs->GetImages();
    for(uint curFrame = 0; curFrame < m_numFrames && curFrame < frames.size(); ++curFrame)
    {
        QImage* img = new QImage(m_width, m_height, QImage::Format_ARGB32);
        QPainter* painter = new QPainter(img);
        frames.at(curFrame)->paintEvent(*painter, false);

        //setup images
        img = new QImage(m_width, m_height, QImage::Format_ARGB32);
        img->fill(0);

        //get frame content
        painter = new QPainter(img);
        frames.at(curFrame)->paintEvent(*painter, false);
        delete painter;
        painter = 0;

        for(unsigned int curCol = 0; curCol < m_width; ++curCol)
        {
            for(unsigned int curRow = 0; curRow < m_height; ++curRow)
            {
                QRgb c2 = img->pixel(curCol, curRow);
                m_colorGrid[curFrame][curRow][curCol].m_colRGBA.a = c2 >> 24;
                m_colorGrid[curFrame][curRow][curCol].m_colRGBA.r = (c2 >> 16) & 255;
                m_colorGrid[curFrame][curRow][curCol].m_colRGBA.g = (c2 >> 8) & 255;
                m_colorGrid[curFrame][curRow][curCol].m_colRGBA.b = c2  & 255;
            }
        }
    }
}

Canvas* ProjectFile::GetContents()
{
    Canvas* canvas = new Canvas(m_width, m_height);
    for(uint frame = 1; frame < m_numFrames; ++frame)
        canvas->AddFrame();
    for(uint curFrame = 0; curFrame < m_numFrames; ++curFrame)
    {
        canvas->SelectFrame(curFrame);
        for(unsigned int curCol = 0; curCol < m_width; ++curCol)
        {
            for(unsigned int curRow = 0; curRow < m_height; ++curRow)
            {
                uint t = (m_colorGrid[curFrame][curRow][curCol].m_colRGBA.a << 24) | (m_colorGrid[curFrame][curRow][curCol].m_colRGBA.r << 16) | (m_colorGrid[curFrame][curRow][curCol].m_colRGBA.g << 8) | (m_colorGrid[curFrame][curRow][curCol].m_colRGBA.b);
                canvas->SetPixel(curCol, curRow, t);
            }
        }

    }
    return canvas;
}
