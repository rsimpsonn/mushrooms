#include "Cone.h"
#include <iostream>
#include "glm/ext.hpp"

Cone::Cone()
{

}

Cone::~Cone()
{
}


Cone::Cone(int param1, int param2, int param3) :
    Shape(param1, glm::max(3, param2), param3),
    m_radius(0.5),
    m_height(1)
{

    m_faces = {
        Circle(m_radius, glm::vec3{0, m_height / 2.0f, 0}, -1),
        Circle(m_radius, glm::vec3{0, -1 * m_height / 2.0f, 0}, 1)
    };

    initializeOpenGLShapeProperties();

}

void Cone::tessellate(int param1, int param2, int param3) {

    param2 = glm::max(3, param2);

    if (m_previousTessellation.size() > 0 && param1 == m_param1 && param2 == m_param2)
    {
        std::cout << "Getting cone from cache" << std::endl;
        m_vertexData = m_previousTessellation;
    } else {
        std::vector<GLfloat> tessellation;
        std::cout << "Redrawing cone" << std::endl;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        for (int i = 0; i < m_faces.size(); i++)
        {
            m_faces[i].spokeBy(m_param2);
            m_faces[i].concentricBy(m_param1);

            std::vector<Triangle> tris = i == 0 ?
                        m_faces[i].getTrianglesDecline(m_height) :
                        m_faces[i].getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                if (i == 0) {
                    Triangle tri = tris[j];
                    tris[j].remapNormals
                            (
                                [](glm::vec3 a) {
                        float r = glm::distance(a, glm::vec3{0, a.y, 0});
                        return glm::normalize(a - glm::vec3{0, a.y - r * 0.5, 0}); },
                    [](glm::vec3 a) {
                        return glm::all(glm::equal(a, glm::vec3{0, a.y, 0}));
                    }
                    );
                }
                tris[j].addToVector(&tessellation);
            }
        }

        m_vertexData = tessellation;
        m_previousTessellation = tessellation;
    }

    std::cout << "initialize" << std::endl;
    initializeOpenGLShapeProperties();
}

std::tuple<bool, float, glm::vec3, glm::vec3> Cone::intersects(glm::vec3 p, glm::vec3 d)
{
    float a = pow(d.x, 2) + pow(d.z, 2) - (0.25f * pow(d.y, 2));
    float b = (2.0f * p.x * d.x) + (2.0f * p.z * d.z) - (0.5f * p.y * d.y) + (0.25f * d.y);
    float c = pow(p.x, 2) + pow(p.z, 2) - (0.25f * pow(p.y, 2)) + (0.25f * p.y) - (1.0f/16);
    float dis = pow(b, 2) - (4.0f * a * c);

    bool passesBody = true;

    float t;
    float y;

    if (dis >= 0) {
        float tNegative = ((-1.0f * b) - sqrt(dis)) / (2.0f*a);
        float negativeY = p.y + d.y * tNegative;
        bool nValid = tNegative >= 0 && negativeY >= -0.5 && negativeY <= 0.5;

        float tPositive = ((-1.0f * b) + sqrt(dis)) / (2.0f*a);
        float positiveY = p.y + d.y * tPositive;
        bool pValid = tPositive >= 0 &&  positiveY >= -0.5 && positiveY <= 0.5;

        if (nValid && pValid) {
            t = std::min(tPositive, tNegative);
            y = p.y + d.y * t;
        } else if (pValid) {
            t = tPositive;
            y = p.y + d.y * t;
        } else if (nValid) {
            t = tNegative;
            y = p.y + d.y * t;
        } else {
            passesBody = false;
        }
    } else {
        passesBody = false;
    }


    float cap2T = (-0.5 - p.y)/d.y;
    float cap2X = p.x + d.x * cap2T;
    float cap2Z = p.z + d.z * cap2T;

    bool passesCap = false;

    if (cap2T >= 0 && (pow(cap2X, 2) + pow(cap2Z, 2)) <= 0.25f) {
        passesCap = true;
    }

    float x = p.x + d.x * t;
    float z = p.z + d.z * t;
    float r = sqrt(pow(x, 2) + pow(z, 2));


    if (passesBody && passesCap) {
        if (t < cap2T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0.5 * r, z}), glm::vec3{x,y,z});
        } else {
            return std::make_tuple(true, cap2T, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X,-0.5,cap2Z});
        }
    } else if (passesBody) {
        return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0.5 * r, z}), glm::vec3{x,y,z});
    } else if (passesCap) {
        return std::make_tuple(true, cap2T, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X,-0.5,cap2Z});
    }

    return std::make_tuple(false, 0.0, glm::vec3{0,0,0}, glm::vec3{0,0,0});

}

std::tuple<float, float> Cone::unitSquare(glm::vec3 coordinate) {
    if (abs(coordinate.y + 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, 1.f - coordinate.z + 0.5);
    // on body

    float theta;
    float r = glm::length(coordinate.xz());

    if (coordinate.x > 0 && coordinate.z < 0) {
        theta = acos((coordinate.x * r + coordinate.z * 0) / pow(r, 2));
    } else if (coordinate.x < 0 && coordinate.z < 0) {
        theta = float(M_PI) / 2.f + acos((coordinate.x * 0 + coordinate.z * -r) / pow(r, 2));
    } else if (coordinate.x < 0 && coordinate.z > 0) {
        theta = float(M_PI) + acos((coordinate.x * -r + coordinate.z * 0) / pow(r, 2));
    } else {
        theta = 3 * float(M_PI) / 2.f + acos((coordinate.x * 0 + coordinate.z * r) / pow(r, 2));
    }

    return std::make_tuple(theta / (2.f * M_PI), 1.f - coordinate.y + 0.5);

}
