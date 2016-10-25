#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "color.h"

class Canvas;

typedef unsigned char uchar;
typedef unsigned int uint;

class ProjectFile
{
public:
    ProjectFile(const char * fileName);
    ProjectFile(const Canvas * cnvs);
    ~ProjectFile();

    bool Save(const char* fileName);
    Canvas* GetContents();
private:
    uint m_height, m_width, m_numFrames;

    void DestroyColorGrid();
    void PopulateColorGrid();

    void GrabContentFromCanvas(const Canvas * cnvs);

    Color*** m_colorGrid;
};

#endif // PROJECTFILE_H
