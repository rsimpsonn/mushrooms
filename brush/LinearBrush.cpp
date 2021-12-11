/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */


#include "LinearBrush.h"

#include <math.h>

#include <algorithm>
#include <iostream>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...
    m_mask.clear();

    int side = 2 * getRadius() + 1;
    int midPoint = getRadius();

    m_mask.reserve(side * side);

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            float dist = sqrt(pow((i - midPoint), 2) + pow((j - midPoint), 2));
            float intensity = 1 - std::min(1.0f, dist / getRadius());
            m_mask.push_back(intensity);
        }
    }
}


