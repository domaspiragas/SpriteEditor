#ifndef COLOR_H
#define COLOR_H

union Color{
    struct RGBA{
        unsigned char a;
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    RGBA m_colRGBA;
    unsigned int m_col;
};

#endif // COLOR_H
