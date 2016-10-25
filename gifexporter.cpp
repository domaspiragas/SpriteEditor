#if GIFEXPORT
#include "gifexporter.h"
#include "Magick++.h"
#include "canvas.h"
#include <string.h>
#include <vector>
#include "frame.h"
#include <QPainter>
#include <vector>
#include <QImage>
#include <sstream>
#include "color.h"

Magick::Color ConvertColor(Color c)
{
    Magick::ColorRGB col(c.m_colRGBA.r/255.0, c.m_colRGBA.g/255.0, c.m_colRGBA.b/255.0);
    return col;
}

GIFExporter::GIFExporter()
{
}

/** I hacked this method together, it runs on the CADE remote machines. */
bool GIFExporter::ExportGifRedo(Canvas* cnvs, const char* filePath, unsigned int delay)
{
    const std::vector<Frame*>& frames = cnvs->GetImages();
    unsigned int len = frames.size();
    QImage* img = 0;
    QPainter* painter = 0;
    uint w, h;
    frames.at(0)->GetDimensions(w, h);
    system("rm temp*.png -f"); // -f there to hide output
    for(unsigned int curFrame = 0; curFrame < len; ++curFrame)
    {
        delete painter;
        delete img;
        img = new QImage(w, h, QImage::Format_ARGB32);;
        img->fill(0);
        painter = new QPainter(img);
        frames.at(curFrame)->paintEvent(*painter, false);
        QString tempFileName;
        if (curFrame < 10)
            tempFileName = QString("00") + QString::number(curFrame);
        else if (curFrame < 100)
            tempFileName = QString("0") + QString::number(curFrame);
        else
            tempFileName = QString::number(curFrame);
        QString temp = "temp" + tempFileName + QString(".png");
        img->save(temp, "PNG");
    }
    QString command("convert -delay ");
    command.append(QString::number(delay)); // delay is int hundreths of seconds
    command.append(" -dispose background temp*.png  -loop 0 ");
    command.append(filePath);
    QByteArray ba = command.toLatin1();
    system(ba.data());
    delete painter;
    painter = 0;
//    system("rm temp*.png");
    return true;
}


bool GIFExporter::ExportGIF(Canvas* cnvs, const char* filePath, unsigned int delay)
{
    const std::vector<Frame*>& frames = cnvs->GetImages();

    unsigned int len = frames.size();
    QImage* img = 0;
    QPainter* painter = 0;

    std::vector<Magick::Image>* magImgs = new std::vector<Magick::Image>();

    uint w, h;
    frames.at(0)->GetDimensions(w, h);

    std::stringstream* dimensions = new std::stringstream();
    *dimensions << (int)w;
    *dimensions << "x";
    *dimensions << (int) h;
    std::string dim("");
    *dimensions >> dim;

    for(unsigned int curFrame = 0; curFrame < len; ++curFrame)
    {
        delete painter;
        delete img;
        img = new QImage(w, h, QImage::Format_ARGB32);;
        img->fill(0);
        painter = new QPainter(img);
        frames.at(curFrame)->paintEvent(*painter, false);

        Magick::Image mImg = Magick::Image(dim.c_str(), "white");
        mImg.transparent(Magick::Color("white"));

        Magick::PixelPacket *packet = mImg.getPixels(0,0,w,h);

        for(unsigned int curCol = 0; curCol < w; ++curCol)
        {
            for(unsigned int curRow = 0; curRow < h; ++curRow)
            {
                QRgb col = img->pixel(curCol, curRow);
                Color rgb;
                rgb.m_colRGBA.a = col >> 24;
                rgb.m_colRGBA.r = (col >> 16) & 255;
                rgb.m_colRGBA.g = (col >> 8) & 255;
                rgb.m_colRGBA.b = col  & 255;

                Magick::Color color (ConvertColor(rgb));

                if(rgb.m_colRGBA.a > 20)
                    *(packet+curRow*w+curCol) = color;
            }
        }
        mImg.animationDelay(delay);
        mImg.syncPixels();
        magImgs->push_back( mImg );
    }

    Magick::writeImages(magImgs->begin(), magImgs->end(), filePath);

    delete painter;
    painter = 0;

    delete magImgs;

    return true;
}

float GIFExporter::Delta(const QColor& c1, const QColor& c2)
{
    int rDelta = std::abs(c1.red() - c2.red());
    int gDelta = std::abs(c1.green() - c2.green());
    int bDelta = std::abs(c1.blue() - c2.blue());
    int aDelta = std::abs(c1.alpha() - c2.alpha());

    int deltaTot = rDelta + gDelta + bDelta + aDelta;
    return deltaTot / 4.0;
}

#endif
