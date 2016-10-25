#include <QColorDialog>
#ifndef COLORPICKER_H
#define COLORPICKER_H


class ColorPicker
{
public:
    ColorPicker(const QColor* col = 0, bool useAlpha = false);
    QColor selectColor();
    QString getRGB();
private:
    QColor m_color;
    bool m_useAlpha;
};

#endif // COLORPICKER_H
