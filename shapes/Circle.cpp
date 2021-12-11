#include "Circle.h"

#include <math.h>
#include "glm/ext.hpp"
#include <iostream>

Circle::Circle(float radius, glm::vec3 center, int orientation) :
    m_radius(radius),
    m_center(center),
    m_orientation(orientation),
    m_concentric(1)
{

}

Circle::~Circle()
{
}

<<<<<<< HEAD
float Circle::getRadius() {
    return m_radius;
}

float Circle::getYVal() {
    return m_center.y;
}

=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3

void Circle::spokeBy(int n)
{
    m_vertices = n;
}

void Circle::concentricBy(int n)
{
    m_concentric = n;
}

std::vector<glm::vec3> Circle::getOuterVertices()
{
    float interval = 2 * M_PI / m_vertices;
    float currAngle = 0;
    std::vector<glm::vec3> vertices;

    for (int i = 0; i < m_vertices; i++)
    {
        vertices.push_back(
                        glm::vec3{
                            m_radius * cos(currAngle),
                            0,
                            m_radius * sin(currAngle)
                        } + m_center);

        currAngle += interval;
    }
    return vertices;

}

std::vector<Triangle> Circle::getTriangles()
{
    return toTriangles([](int a) { return 0; });
}

std::vector<Triangle> Circle::getTrianglesDecline(float n)
{
    return toTriangles([this, n](int a) { return -1 * (float(a + 1) / m_concentric) * n; });
}

std::vector<Triangle> Circle::toTriangles(std::function<float (int)> func)
{
    std::vector<std::vector<glm::vec3>> vertices;
    float interval = m_orientation * 2 * M_PI / m_vertices;
    float currAngle = 0;

    for (int i = 0; i < m_vertices; i++)
    {
        std::vector<glm::vec3> verticesAtAngle;

        verticesAtAngle.push_back(m_center);

        for (int j = 0; j < m_concentric; j++)
        {
            verticesAtAngle.push_back(
                        glm::vec3{
                            m_radius * (float(j + 1) / m_concentric) * cos(currAngle),
                            func(j),
                            m_radius * (float(j + 1) / m_concentric) * sin(currAngle)
                        } + m_center);
        }

        vertices.push_back(verticesAtAngle);
        currAngle += interval;
    }

    std::vector<Triangle> triangles;
    for (int i = 0; i < vertices.size(); i++)
    {
        std::vector<glm::vec3> side1 = vertices[i];
        std::vector<glm::vec3> side2 = vertices[(i + 1) % vertices.size()];

        for (int j = 0; j < m_concentric; j++)
        {
            glm::vec3 p1 = side1[j];
            glm::vec3 p2 = side2[j];
            glm::vec3 p3 = side2[j + 1];
            glm::vec3 p4 = side1[j + 1];

            Triangle tri1 = Triangle(p2, p4, p1);
            Triangle tri2 = Triangle(p4, p2, p3);

            triangles.push_back(tri1);
            triangles.push_back(tri2);
        }
    }

    return triangles;
}
