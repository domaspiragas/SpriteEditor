#include "bucketfill.h"
#include <unordered_map>

std::string BucketFill::GetPointString(QPoint& p)
{
    std::stringstream s("");
    s << p.x() << "," << p.y();
    std::string result;
    s >> result;
    return result;
}

bool BucketFill::IsChecked(QPoint& p, std::unordered_map<std::string, QPoint>& map)
{
    return map.find(GetPointString(p)) != map.end();
}

void BucketFill::AddItemToList(QPoint& p, std::list<QPoint>& open, std::unordered_map<std::string, QPoint>& map)
{
    if(!IsChecked(p, map))
    {
        std::pair<std::string, QPoint> insertPair(GetPointString(p), p);
        map.insert(insertPair);
        open.push_back(p);
    }
}

void BucketFill::RunThroughList(std::list<QPoint>& openList, std::unordered_map<std::string, QPoint>& closedList, const QColor& targetColor, const QColor& col, QImage* img)
{
    if(openList.size() > 0)
    {
        QPoint p = openList.front();
        openList.pop_front();
        img->setPixel(p, targetColor.rgb());

        QPoint north = QPoint(p.x(), p.y() - 1);
        QPoint south = QPoint(p.x(), p.y() + 1);
        QPoint west = QPoint(p.x() - 1, p.y());
        QPoint east = QPoint(p.x() + 1, p.y());

        if(north.y() >= 0 && QColor(img->pixel(north)) == col)
        {
            AddItemToList(north, openList, closedList);
        }

        if(south.y() < img->height() && QColor(img->pixel(south)) == col)
        {
            AddItemToList(south, openList, closedList);
        }

        if(west.x() >= 0 && QColor(img->pixel(west)) == col)
        {
            AddItemToList(west, openList, closedList);
        }

        if(east.x() < img->width() && QColor(img->pixel(east)) == col)
        {
            AddItemToList(east, openList, closedList);
        }
        RunThroughList(openList, closedList, targetColor, col, img);
    }
}

void BucketFill::Fill(int x, int y, const QColor& targetColor, QImage *img)
{
        QPoint p(x,y);
    QColor col = img->pixel(p);

    std::list<QPoint> openList = std::list<QPoint>();
    std::unordered_map<std::string, QPoint> closedList = std::unordered_map<std::string, QPoint>();

    AddItemToList(p, openList, closedList);
    RunThroughList(openList, closedList, targetColor, col, img);
}

