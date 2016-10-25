#include "tool.h"

#include <QPen>
#include <QPainter>

Tool::Tool()
{

}


/** Set the pen type t use */
void Tool::SetPen(const QColor * col, QPainter * qp, unsigned int penSize)
{
    if(col && qp)
    {
        QPen pen(*col, penSize);
        qp->setPen(pen);
    }
}

void Tool::SetBrush(const QColor* fillCol, QPainter* qp)
{
    if(fillCol && qp)
    {
        QBrush b(*fillCol);
        qp->setBrush(b);
    }
}
