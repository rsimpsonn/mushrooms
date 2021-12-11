/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include <algorithm>

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...

    m_mask.clear();

    int side = 2 * getRadius() + 1;
    int midPoint = getRadius();

    m_mask.reserve(side * side);

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            float dist = sqrt(pow((i - midPoint), 2) + pow((j - midPoint), 2));
            if (dist < getRadius()) {
                m_mask.push_back(1);
            } else {
                m_mask.push_back(0);
            }
        }
    }
}


