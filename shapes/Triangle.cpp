#include "Triangle.h"
<<<<<<< HEAD
#include "glm/ext.hpp"
#include <iostream>

Triangle::Triangle()
{}
=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    addVertex(p1, p2, p3);
    addVertex(p3, p1, p2);
    addVertex(p2, p3, p1);
}

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, std::function<glm::vec3 (glm::vec3 p)> func)
{
    addVertex(p1, func);
    addVertex(p3, func);
    addVertex(p2, func);
}

Triangle::~Triangle()
{
}

void Triangle::addVertex(glm::vec3 src, glm::vec3 dest1, glm::vec3 dest2)
{
    glm::vec3 leg1 = dest1 - src;
    glm::vec3 leg2 = dest2 - src;

    glm::vec3 normal = glm::normalize(glm::cross(leg2, leg1));

    m_normals.push_back(normal);
    m_vertices.push_back(src);
}

void Triangle::addVertex(glm::vec3 src, std::function<glm::vec3 (glm::vec3 p)> func)
{
    m_normals.push_back(func(src));
    m_vertices.push_back(src);
}

void Triangle::remapNormals(std::function<glm::vec3 (glm::vec3 p)> func)
{
    m_normals.clear();

    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_normals.push_back(func(m_vertices[i]));
    }
}

void Triangle::remapNormals(std::function<glm::vec3 (glm::vec3 p)> func, std::function<bool (glm::vec3 p)> ignoreIf)
{

    std::vector<glm::vec3> updated;

    for (int i = 0; i < m_vertices.size(); i++)
    {
        if (!ignoreIf(m_vertices[i]))
        {
            updated.push_back(func(m_vertices[i]));
        } else {
            updated.push_back(m_normals[i]);
        }
    }

    m_normals = updated;
}

std::vector<glm::vec3> Triangle::getNormals()
{
    return m_normals;
}

std::vector<glm::vec3> Triangle::getVertices()
{
    return m_vertices;
}

void Triangle::addToVector(std::vector<GLfloat> *v)
{
    std::vector<glm::vec3> vertices = getVertices();
    std::vector<glm::vec3> normal = getNormals();

    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            v->push_back(vertices[k][l]);
        }
        for (int l = 0; l < 3; l++) {
            v->push_back(normal[k][l]);
        }
    }

}
<<<<<<< HEAD

void Triangle::addToVector(std::vector<GLfloat> *v, glm::mat4 t)
{
    std::vector<glm::vec3> vertices = getVertices();
    std::vector<glm::vec3> normal = getNormals();


    for (int k = 0; k < vertices.size(); k++) {
        glm::vec4 transformed = t * glm::vec4(vertices[k], 1);
        vertices[k] = transformed.xyz();
    }

    for (int k = 0; k < normal.size(); k++) {
        glm::vec4 transformed = t * glm::vec4(normal[k], 0);
        normal[k] = transformed.xyz();
    }

    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            v->push_back(vertices[k][l]);
        }
        for (int l = 0; l < 3; l++) {
            v->push_back(normal[k][l]);
        }
    }

}
=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
