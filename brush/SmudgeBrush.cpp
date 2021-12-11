/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"
#include <iostream>


SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

    m_mask.clear();

    int side = 2 * getRadius() + 1;
    int midPoint = getRadius();

    m_mask.reserve(side * side);

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            float dist = sqrt(pow((i - midPoint), 2) + pow((j - midPoint), 2));
            float quad_a = 1.0f / pow(getRadius(), 2);
            float quad_b = -2.0f / getRadius();
            if (dist <= getRadius()) {
                m_mask.push_back(quad_a * pow(dist, 2) + quad_b * dist + 1);
            } else {
                m_mask.push_back(0);
            }
        }
    }
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

    RGBA *pix = canvas->data();

    int rowStart = y - getRadius();
    int rowEnd = y + getRadius();

    int colStart = x - getRadius();
    int colEnd = x + getRadius();

    std::vector<RGBA> paint;

    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = colStart; j < colEnd; j++) {
            int index = ((i - 1) * canvas->width()) + (j + 1) - 1;
            RGBA pixel = pix[index];
            paint.push_back(pixel);
        }
    }

    m_paint = std::vector<RGBA>(paint);

}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // now pick up paint again...

    RGBA *pix = canvas->data();

    int rowStart = std::max(0, mouseY - getRadius());
    int rowEnd = std::min(canvas->height() - 1, mouseY + getRadius());

    int colStart = std::max(0, mouseX - getRadius());
    int colEnd = std::min(canvas->width() - 1, mouseX + getRadius());

    int prevIndex = 0;

    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = colStart; j < colEnd; j++) {
            int index = ((i - 1) * canvas->width()) + (j + 1) - 1;
            RGBA *pixel = &pix[index];
            RGBA prevPixel = m_paint[prevIndex];
            prevIndex += 1;

            int offsetX = j - mouseX;
            int offestY = i - mouseY;
            float maskIntensity = getMaskIntensity(offsetX, offestY);
            //float alphaRatio = float(getAlpha()) / 256;

            pixel->r = maskIntensity * prevPixel.r + (1 - maskIntensity) * pixel->r;
            pixel->b = maskIntensity * prevPixel.b + (1 - maskIntensity) * pixel->b;
            pixel->g = maskIntensity * prevPixel.g + (1 - maskIntensity) * pixel->g;
        }
    }

    canvas->update();

    pickUpPaint(mouseX, mouseY, canvas);

}


