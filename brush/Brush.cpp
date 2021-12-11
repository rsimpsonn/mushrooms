/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"
#include <algorithm>
#include <iostream>

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}

float Brush::getMaskIntensity(int offsetX, int offsetY) {
    int midPoint = getRadius();
    int row = midPoint + offsetY;
    int col = midPoint + offsetX;
    int index = row * (2 * getRadius() + 1) + col;
    return m_mask[index];
}




void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    RGBA *pix = canvas->data();

    int rowStart = std::max(0, mouseY - getRadius());
    int rowEnd = std::min(canvas->height() - 1, mouseY + getRadius());

    int colStart = std::max(0, mouseX - getRadius());
    int colEnd = std::min(canvas->width() - 1, mouseX + getRadius());

    std::cout << "Canvas2d::mouseDown() called " << mouseX << ", " << mouseY << std::endl;
    std::cout << "BRUSH " << getRed() << ", " << getBlue() << ", " << getGreen() << ", " << getAlpha() << std::endl;

    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = colStart; j < colEnd; j++) {
            int index = ((i - 1) * canvas->width()) + (j + 1) - 1;
            RGBA *pixel = &pix[index];

            int offsetX = j - mouseX;
            int offestY = i - mouseY;
            float maskIntensity = getMaskIntensity(offsetX, offestY);
            float alphaRatio = float(getAlpha()) / 256;

            pixel->r = alphaRatio * maskIntensity * getRed() + (1 - alphaRatio * maskIntensity) * pixel->r;
            pixel->b = alphaRatio * maskIntensity * getBlue() + (1 - alphaRatio * maskIntensity) * pixel->b;
            pixel->g = alphaRatio * maskIntensity * getGreen() + (1 - alphaRatio * maskIntensity) * pixel->g;
        }
    }

    canvas->update();

}
