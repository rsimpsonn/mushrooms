#include "Sphere.h"
#include <iostream>
#include "Rectangle.h"

Sphere::Sphere()
{

}

Sphere::Sphere(int param1, int param2, int param3) :
    Shape(glm::max(2, param1), glm::max(3, param2), param3),
    m_radius(0.5)
{

    setLats();

}

Sphere::~Sphere()
{
}

void Sphere::setLats()
{
    m_lats.clear();
    float angle = M_PI / 2.0f;
    float interval = M_PI / (m_param1);

    for (int i = 0; i < m_param1 + 1; i++)
    {
        float x = m_radius * cos(angle);
        float z = m_radius * sin(angle);
        m_lats.push_back(Circle(x, glm::vec3{0, z, 0}, -1));
        angle -= interval;
    }
}

void Sphere::tessellate(int param1, int param2, int param3) {

    param1 = glm::max(2, param1);
    param2 = glm::max(3, param2);

    if (m_previousTessellation.size() > 0 && param1 == m_param1 && param2 == m_param2)
    {
        std::cout << "Getting sphere from cache" << std::endl;
        m_vertexData = m_previousTessellation;
    } else {
        std::cout << "Redrawing sphere" << std::endl;
        std::vector<GLfloat> tessellation;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        setLats();

        for (int i = 0; i < m_lats.size(); i++)
        {
            m_lats[i].spokeBy(m_param2);
        }

        for (int i = 0; i < m_lats.size() - 1; i++)
        {
            std::vector<glm::vec3> outerVertices = m_lats[i + 1].getOuterVertices();
            std::vector<glm::vec3> outerVerticesNext = m_lats[i].getOuterVertices();

            for (int j = 0; j < outerVertices.size(); j++)
            {
                glm::vec3 p1 = outerVertices[j];
                glm::vec3 p2 = outerVertices[(j + 1) % outerVertices.size()];
                glm::vec3 p3 = outerVerticesNext[(j + 1) % outerVertices.size()];
                glm::vec3 p4 = outerVerticesNext[j];

                Triangle tri1 = Triangle(p2, p3, p1, [](glm::vec3 a) { return glm::normalize(a - glm::vec3{0, 0, 0}); });
                tri1.addToVector(&tessellation);

                Triangle tri2 = Triangle(p1, p3, p4, [](glm::vec3 a) { return glm::normalize(a - glm::vec3{0, 0, 0}); });
                tri2.addToVector(&tessellation);

            }

        }
        m_vertexData = tessellation;
        m_previousTessellation = tessellation;
    }

    initializeOpenGLShapeProperties();
}


std::tuple<bool, float, glm::vec3, glm::vec3> Sphere::intersects(glm::vec3 p, glm::vec3 d)
{
    float a = pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2);
    float b = (2.0f * p.x * d.x) + (2.0f * p.y * d.y) + (2.0f * p.z * d.z);
    float c = -0.25f + pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2);

    float dis = pow(b, 2) - (4.0f * a * c);
    if (dis < 0) {
        return std::make_tuple(false, 0.0, glm::vec3{0,0,0}, glm::vec3{0,0,0});
    }

    float tNegative = (-1.0f * b - sqrt(dis)) / (2.0f*a);
    float tPositive = (-1.0f * b + sqrt(dis)) / (2.0f*a);

    float t;
    if (tNegative >= 0 && tPositive >= 0) {
        t = std::min(tNegative, tPositive);
    } else if (tNegative >= 0) {
        t = tNegative;
    } else if (tPositive >= 0) {
        t = tPositive;
    } else {
        return std::make_tuple(false, 0.0, glm::vec3{0,0,0}, glm::vec3{0,0,0});
    }

    float x = p.x + d.x*t;
    float y = p.y + d.y*t;
    float z = p.z + d.z*t;

    return std::make_tuple(true, t, glm::normalize(glm::vec3{x, y, z}), glm::vec3{x, y, z});

}

std::tuple<float, float> Sphere::unitSquare(glm::vec3 coordinate) {

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

    float phi = asin(-coordinate.y / 0.5);

    return std::make_tuple(theta / (2.f * M_PI), phi / M_PI + 0.5);

}
