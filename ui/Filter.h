#ifndef FILTER_H
#define FILTER_H

#include "Canvas2D.h"


class Filter
{
public:
    Filter();

    static void grayscale(Canvas2D *canvas);
    static void edgeDetect(Canvas2D *canvas, float sensitivity);
    static void blur(Canvas2D* canvas, int r);
    static void scaleHorizontal(Canvas2D* canvas, float x);
    static void scaleVertical(Canvas2D* canvas, float y);
};

#endif // FILTER_H
