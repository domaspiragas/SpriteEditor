#ifndef BUCKETFILL_H
#define BUCKETFILL_H

#include <QPoint>
#include <QColor>
#include <QImage>
#include <unordered_map>
#include <sstream>


/**
 * @brief This isn't a normal tool since the API is going to e too vastly different; it doesn't need a state so it'll be static
 */
class BucketFill
{
    BucketFill(){};
public:
    static void Fill(int x, int y, const QColor& targetColor, QImage* img);
private:
    static void RunThroughList(std::list<QPoint>& openList, std::unordered_map<std::string, QPoint>& closedList, const QColor& targetColor, const QColor& col, QImage* img);
    static void AddItemToList(QPoint& p, std::list<QPoint>& open, std::unordered_map<std::string, QPoint>& map);
    static bool IsChecked(QPoint& p, std::unordered_map<std::string, QPoint>& map);
    static std::string GetPointString(QPoint& p);
};

#endif // BUCKETFILL_H
