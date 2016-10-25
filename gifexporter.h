#ifndef GIFEXPORTER_H
#define GIFEXPORTER_H
#include <string>
#include <QColor>

#if GIFEXPORT
class Canvas;

class GIFExporter
{
private:
    GIFExporter();
public:
    static bool ExportGIFStr(Canvas* cnvs, const std::string& filePath, unsigned int delay){
        return ExportGIF(cnvs, filePath.c_str(), delay);
    }

    static bool ExportGIF(Canvas* cnvs, const char* filePath, unsigned int delay);
    static bool ExportGifRedo(Canvas* cnvs, const char* filePath, unsigned int delay);

private:
    static float Delta(const QColor&, const QColor&);
};
#endif

#endif // GIFEXPORTER_H
