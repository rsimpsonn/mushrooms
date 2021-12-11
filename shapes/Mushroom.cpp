#include "Mushroom.h"
#include "Cylinder.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Settings.h"
#include <iostream>
#include "CS123SceneData.h"

Mushroom::Mushroom(int parameter1, int parameter2, int mushroomType):
    m_topCircles(std::vector<Circle>()),
    m_stemCircles(std::vector<Circle>())
{
    // adjust parameters so it looks normal

}

void Mushroom::makeUmbrellaShroom(int p1, int p2) {
    // mushroom top
    float thetaInterval = M_PI/(float)p1;

    for (float i=0; i<=2 * M_PI; i+=thetaInterval) {
        float currentTheta = (M_PI/2.0f) - i;
        float newRadius = abs(1.f*cos(currentTheta));
        float yVal = 1.f*sin(currentTheta);
        m_topCircles.push_back(Circle(newRadius, glm::vec3(0,yVal,0), 1));
        if (yVal < 0) {
            break;
        }
    }

    for (float i=0; i<=1; i+=0.1f) {
        float newRadius = ((0.3f*-cos(3.75f*i))+1.4f) / 5.f;
        float yVal = -i;
        m_stemCircles.push_back(Circle(newRadius, glm::vec3(0,yVal,0), 1));
    }

}

void Mushroom::makeBellShroom(int p1, int p2) {

    // mushroom top
    for (float i=0.4627f; i<=1.f; i+=0.0001f) {
        float newRadius;
        if (i<=0.5084f) {
            newRadius = pow((0.15f-pow((i-0.85f),2.f)),0.5f);
        } else {
            newRadius = 0.29f*(-cos(-3.75f*i)+0.3f);
        }
        float yVal = 1.f-i;
        m_topCircles.push_back(Circle(newRadius, glm::vec3(-15.f,yVal,0), 1));
        if (yVal < 0) {
            break;
        }
    }

    for (float i=-0.1f; i<=1.7f; i+=0.1f) {
        float newRadius = (0.02f*(-cos(-9.f*i)+0.2f))+0.05f;
        float yVal = -i;
        m_stemCircles.push_back(Circle(newRadius, glm::vec3(-15.f,yVal,0), 1));
    }

}

void Mushroom::makePancakeShroom(int p1, int p2) {

    // mushroom top
    for (float i=0.4627f; i<=1.f; i+=0.01f) {
        float newRadius;
        if (i<=0.5084f) {
            newRadius = pow((0.15f-pow((i-0.85f),2.f)),0.5f) + 3.f;
        } else {
            newRadius = 0.29f*(-cos(-3.75f*i)+0.3f) * 3.f;
        }
        float yVal = 1.f-i;
        m_topCircles.push_back(Circle(newRadius, glm::vec3(-15.f,yVal,0), 1));
        if (yVal < 0) {
            break;
        }
    }

    for (float i=-0.1f; i<=1.7f; i+=0.1f) {
        float newRadius = (0.1f*(-cos(-3.95f*i)+0.3f))+0.5f;
        float yVal = -i;
        m_stemCircles.push_back(Circle(newRadius, glm::vec3(-15.f,yVal,0), 1));
    }

}

void Mushroom::tessellate(int parameter1, int parameter2, int parameter3) {
    if (parameter2 < 3) {
        parameter2 =10;
    }

    if (parameter1 < 2) {
        parameter1 = 15;
    }

    switch (parameter3) {

        case 1:
            makeUmbrellaShroom(parameter1, parameter2);
        break;
    default:
        makeUmbrellaShroom(parameter1, parameter2);
//        case 2:
//            makeBellShroom(parameter1, parameter2);
//        case 3:
//            makePancakeShroom(parameter1, parameter2);

    }

    float interval = float(2.0f*M_PI)/parameter2;

    // connect circles of mushroom top
    for (float i=0; i<(2.0f*M_PI); i+=interval) {
        for (int j=1; j<m_topCircles.size(); j++) {
            glm::vec3 currentCircle1 = glm::vec3(m_topCircles[j].getRadius()*cos(i), m_topCircles[j].getYVal(), -m_topCircles[j].getRadius()*sin(i));
            glm::vec3 currentCircle2 = glm::vec3(m_topCircles[j].getRadius()*cos(i+interval), m_topCircles[j].getYVal(), -m_topCircles[j].getRadius()*sin(i+interval));
            glm::vec3 prevCircle1 = glm::vec3(m_topCircles[j-1].getRadius()*cos(i), m_topCircles[j-1].getYVal(), -m_topCircles[j-1].getRadius()*sin(i));
            glm::vec3 prevCircle2 = glm::vec3(m_topCircles[j-1].getRadius()*cos(i+interval), m_topCircles[j-1].getYVal(), -m_topCircles[j-1].getRadius()*sin(i+interval));
            Triangle tri1 = Triangle(currentCircle1,prevCircle1,currentCircle2);
            tri1.addToVector(&m_vertexData);
            Triangle tri2 = Triangle(currentCircle2,prevCircle1,prevCircle2);
            tri2.addToVector(&m_vertexData);
            if (j == m_topCircles.size()-1) {
                Triangle gill = Triangle(currentCircle2,glm::vec3(0,0,0),currentCircle1);
                gill.addToVector(&m_vertexData);
            }
        }
    }

    // connect circles of mushroom stem
    for (float i=0; i<(2.0f*M_PI); i+=interval) {
        for (int j=0; j<m_stemCircles.size(); j++) {
            glm::vec3 currentCircle1 = glm::vec3(m_stemCircles[j].getRadius()*cos(i), m_stemCircles[j].getYVal(), -m_stemCircles[j].getRadius()*sin(i));
            glm::vec3 currentCircle2 = glm::vec3(m_stemCircles[j].getRadius()*cos(i+interval), m_stemCircles[j].getYVal(), -m_stemCircles[j].getRadius()*sin(i+interval));
            glm::vec3 prevCircle1 = glm::vec3(m_stemCircles[j-1].getRadius()*cos(i), m_stemCircles[j-1].getYVal(), -m_stemCircles[j-1].getRadius()*sin(i));
            glm::vec3 prevCircle2 = glm::vec3(m_stemCircles[j-1].getRadius()*cos(i+interval), m_stemCircles[j-1].getYVal(), -m_stemCircles[j-1].getRadius()*sin(i+interval));
            Triangle tri1 = Triangle(currentCircle1,prevCircle1,currentCircle2);
//            std::vector<glm::vec3> tri1Points = tri1.getCurvedTriangle({glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0)});
            tri1.addToVector(&m_vertexData);
            Triangle tri2 = Triangle(currentCircle2,prevCircle1, prevCircle2);
//            std::vector<glm::vec3> tri2Points = tri2.getCurvedTriangle({glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0)});
            tri2.addToVector(&m_vertexData);

        }
    }


    initializeOpenGLShapeProperties();
}

std::vector<Triangle> Mushroom::getTriangles(int parameter1, int parameter2, int parameter3) {
    if (parameter2 < 3) {
        parameter2 =10;
    }

    if (parameter1 < 2) {
        parameter1 = 15;
    }

    switch (parameter3) {

        case 1:
            makeUmbrellaShroom(parameter1, parameter2);
        break;
    default:
        makeUmbrellaShroom(parameter1, parameter2);
//        case 2:
//            makeBellShroom(parameter1, parameter2);
//        case 3:
//            makePancakeShroom(parameter1, parameter2);

    }

    float interval = float(2.0f*M_PI)/parameter2;

    std::vector<Triangle> out;

    // connect circles of mushroom top
    for (float i=0; i<(2.0f*M_PI); i+=interval) {
        for (int j=1; j<m_topCircles.size(); j++) {
            glm::vec3 currentCircle1 = glm::vec3(m_topCircles[j].getRadius()*cos(i), m_topCircles[j].getYVal(), -m_topCircles[j].getRadius()*sin(i));
            glm::vec3 currentCircle2 = glm::vec3(m_topCircles[j].getRadius()*cos(i+interval), m_topCircles[j].getYVal(), -m_topCircles[j].getRadius()*sin(i+interval));
            glm::vec3 prevCircle1 = glm::vec3(m_topCircles[j-1].getRadius()*cos(i), m_topCircles[j-1].getYVal(), -m_topCircles[j-1].getRadius()*sin(i));
            glm::vec3 prevCircle2 = glm::vec3(m_topCircles[j-1].getRadius()*cos(i+interval), m_topCircles[j-1].getYVal(), -m_topCircles[j-1].getRadius()*sin(i+interval));
            Triangle tri1 = Triangle(currentCircle1,prevCircle1,currentCircle2);
            out.push_back(tri1);
            Triangle tri2 = Triangle(currentCircle2,prevCircle1,prevCircle2);
            out.push_back(tri2);
            if (j == m_topCircles.size()-1) {
                Triangle gill = Triangle(currentCircle2,glm::vec3(0,0,0),currentCircle1);
                out.push_back(gill);
            }
        }
    }

    // connect circles of mushroom stem
    for (float i=0; i<(2.0f*M_PI); i+=interval) {
        for (int j=0; j<m_stemCircles.size(); j++) {
            glm::vec3 currentCircle1 = glm::vec3(m_stemCircles[j].getRadius()*cos(i), m_stemCircles[j].getYVal(), -m_stemCircles[j].getRadius()*sin(i));
            glm::vec3 currentCircle2 = glm::vec3(m_stemCircles[j].getRadius()*cos(i+interval), m_stemCircles[j].getYVal(), -m_stemCircles[j].getRadius()*sin(i+interval));
            glm::vec3 prevCircle1 = glm::vec3(m_stemCircles[j-1].getRadius()*cos(i), m_stemCircles[j-1].getYVal(), -m_stemCircles[j-1].getRadius()*sin(i));
            glm::vec3 prevCircle2 = glm::vec3(m_stemCircles[j-1].getRadius()*cos(i+interval), m_stemCircles[j-1].getYVal(), -m_stemCircles[j-1].getRadius()*sin(i+interval));
            Triangle tri1 = Triangle(currentCircle1,prevCircle1,currentCircle2);
//            std::vector<glm::vec3> tri1Points = tri1.getCurvedTriangle({glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0)});
            out.push_back(tri1);
            Triangle tri2 = Triangle(currentCircle2,prevCircle1, prevCircle2);
//            std::vector<glm::vec3> tri2Points = tri2.getCurvedTriangle({glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0), glm::vec3(0,-(0.1f*j),0)});
            out.push_back(tri2);

        }
    }


    return out;
}

Mushroom::~Mushroom()
{
}
