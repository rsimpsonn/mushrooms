/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "camera/Camera.h"
#include "glm/ext.hpp"
#include "Frustum.h"
#include "Filter.h"

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brush(nullptr),
    m_brushRadius(settings.brushRadius),
    m_brushColor(settings.brushColor),
    m_brushType(settings.brushType)
{
    switch (settings.brushType) {
    case BRUSH_LINEAR:
        m_brush = std::make_unique<LinearBrush>(LinearBrush(m_brushColor, m_brushRadius));
        return;
    case BRUSH_CONSTANT:
        m_brush = std::make_unique<ConstantBrush>(ConstantBrush(m_brushColor, m_brushRadius));
        return;
    case BRUSH_QUADRATIC:
        m_brush = std::make_unique<QuadraticBrush>(QuadraticBrush(m_brushColor, m_brushRadius));
        return;
    case BRUSH_SMUDGE:
        m_brush = std::make_unique<SmudgeBrush>(SmudgeBrush(m_brushColor, m_brushRadius));
        return;
    }
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

    if (settings.brushColor != m_brushColor) {
        m_brushColor = settings.brushColor;
        m_brush.get()->setRGBA(m_brushColor);
    }

    if (settings.brushRadius != m_brushRadius) {
        m_brushRadius = settings.brushRadius;
        m_brush->setRadius(m_brushRadius);
    }

    if (settings.brushType != m_brushType) {
        m_brushType = settings.brushType;
        switch (m_brushType) {
        case BRUSH_LINEAR:
            m_brush = std::make_unique<LinearBrush>(LinearBrush(m_brushColor, m_brushRadius));
            return;
        case BRUSH_CONSTANT:
            m_brush = std::make_unique<ConstantBrush>(ConstantBrush(m_brushColor, m_brushRadius));
            return;
        case BRUSH_QUADRATIC:
            m_brush = std::make_unique<QuadraticBrush>(QuadraticBrush(m_brushColor, m_brushRadius));
            return;
        case BRUSH_SMUDGE:
            m_brush = std::make_unique<SmudgeBrush>(SmudgeBrush(m_brushColor, m_brushRadius));
            return;
        }
    }
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;
    m_brush->brushDown(x, y, this);


//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;
    m_brush->brushDragged(x, y, this);
}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
    m_brush->brushUp(x, y, this);
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch(settings.filterType) {
        case FILTER_EDGE_DETECT:
            Filter::grayscale(this);
            Filter::edgeDetect(this, settings.edgeDetectSensitivity);
            break;
        case FILTER_BLUR:
            Filter::blur(this, settings.blurRadius);
            break;
        case FILTER_SCALE:
            Filter::scaleHorizontal(this, settings.scaleX);
            Filter::scaleVertical(this, settings.scaleY);
    }

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {
    std::cout << "called" << std::endl;
    this->resize(width, height);
    if (m_rayScene) {
        m_rayScene->printObjects();
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
        float U;
        float V;
        float k = 0.5;
        glm::mat4x4 invCameraT;

        std::tie(U, V, invCameraT) = Frustum::generateFrustum(camera, width, height, k);

        RGBA *pix = this->data();

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                float x = (float(j) + 0.5f)/width - 0.5;
                float y = (float(i) + 0.5f)/height - 0.5;
                glm::vec3 uvk = {U * x, V * y, -k};
                glm::vec3 rayCameraSpace = uvk;
                glm::vec4 rayWorldSpace = (glm::vec4{rayCameraSpace, 0.0f} * invCameraT);
                glm::vec3 d = glm::normalize(glm::vec3{rayWorldSpace.x, rayWorldSpace.y, rayWorldSpace.z});

                if (i == 0 && j == 0) {
                    std::cout << "top left corner o " << glm::to_string(rayCameraSpace) << std::endl;
                    std::cout << "top left corner " << glm::to_string(d) << std::endl;
                } else if (i == 0 && j == width - 1) {
                    std::cout << "top right corner o " << glm::to_string(rayCameraSpace) << std::endl;
                    std::cout << "top right corner " << glm::to_string(d) << std::endl;
                } else if (i == height - 1 && j == 0) {
                    std::cout << "bot left corner o " << glm::to_string(rayCameraSpace) << std::endl;
                    std::cout << "bot left corner " << glm::to_string(d) << std::endl;
                } else if (i == height - 1 && j == width - 1) {
                    std::cout << "bot right corner o " << glm::to_string(rayCameraSpace) << std::endl;
                    std::cout << "bot right corner " << glm::to_string(d) << std::endl;
                }

                bool intersects;
                RGBA color;
                std::tie(intersects, color) = m_rayScene->intersect(camera->pos, glm::vec4{d, 0.0f}, *camera);

                int index = ((height - i - 1) * width) + (j + 1) - 1;

                if (index % 1000 == 0) {
                    std::cout << index << "/" << height * width - 1 << std::endl;
                }

                RGBA *pixel = &pix[index];
                if (intersects) *pixel = color;
            }
        }

        this->update();
    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




