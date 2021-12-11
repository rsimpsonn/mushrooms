/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>
#include <algorithm>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...

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


