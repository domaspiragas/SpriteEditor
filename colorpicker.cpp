#include "colorpicker.h"

ColorPicker::ColorPicker(const QColor* col, bool useAlpha)
{
    if(col){
        m_color = *col;
    }
    m_useAlpha = useAlpha;
}

QColor ColorPicker::selectColor()
{

    QColor col;
    if(m_useAlpha)
        col = QColorDialog::getColor(m_color, 0, "Select Color", QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
    else
        col = QColorDialog::getColor(m_color, 0, "Select Color", QColorDialog::DontUseNativeDialog);
    if(col.isValid())
        m_color = col;
    return m_color;
}

QString ColorPicker::getRGB()
{
    //gets the value as an rgb and returns it as a QString that is preformated for use in the SetStyleSheet() method of the UI
    return QString("background-color: rgb(") + QString::number(m_color.red()) + QString(", ") + QString::number(m_color.green()) + QString(", ") + QString::number(m_color.blue()) + QString(");");
}

