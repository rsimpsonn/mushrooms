#include "Cube.h"
#include <iostream>
#include <algorithm>
#include "glm/ext.hpp"

Cube::Cube()
{

}


Cube::Cube(int param1, int param2, int param3) :
    Shape(param1, param2, param3),
    m_sideLength(1)
{

    float half = m_sideLength / 2.0f;
    float nHalf = -1 * half;

    m_faces = {
        Rectangle(glm::vec3{nHalf, half, nHalf}, glm::vec3{half, half, nHalf}, glm::vec3{half, nHalf, nHalf}, glm::vec3{nHalf, nHalf, nHalf}),
        Rectangle(glm::vec3{nHalf, nHalf, half}, glm::vec3{half, nHalf, half}, glm::vec3{half, half, half}, glm::vec3{nHalf, half, half}),
        Rectangle(glm::vec3{nHalf, half, half}, glm::vec3{nHalf, half, nHalf}, glm::vec3{nHalf, nHalf, nHalf}, glm::vec3{nHalf, nHalf, half}),
        Rectangle(glm::vec3{half, nHalf, half}, glm::vec3{half, nHalf, nHalf}, glm::vec3{half, half, nHalf}, glm::vec3{half, half, half}),
        Rectangle(glm::vec3{half, half, half}, glm::vec3{half, half, nHalf}, glm::vec3{nHalf, half, nHalf}, glm::vec3{nHalf, half, half}),
        Rectangle(glm::vec3{nHalf, nHalf, half}, glm::vec3{nHalf, nHalf, nHalf}, glm::vec3{half, nHalf, nHalf}, glm::vec3{half, nHalf, half})
    };

    initializeOpenGLShapeProperties();

}

Cube::~Cube()
{
}

void Cube::tessellate(int param1, int param2, int param3) {

    if (m_previousTessellation.size() > 0 && param1 == m_param1)
    {
        std::cout << "Getting cube from cache" << std::endl;
        m_vertexData = m_previousTessellation;
    } else {
        std::cout << "Redrawing cube" << std::endl;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        std::vector<GLfloat> tessellation;

        for (int i = 0; i < m_faces.size(); i++)
        {
            m_faces[i].splitBy(m_param1);

            std::vector<Triangle> tris = m_faces[i].getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                tris[j].addToVector(&tessellation);
            }
        }

        m_vertexData = tessellation;
        m_previousTessellation = tessellation;
    }

    initializeOpenGLShapeProperties();
}



std::tuple<bool, float, glm::vec3, glm::vec3> Cube::intersects(glm::vec3 p, glm::vec3 d)
{
    float tXLeftBound = (-0.5 - p.x)/d.x;
    float tXRightBound = (0.5 - p.x)/d.x;
    float tXLowerBound = std::min(tXLeftBound, tXRightBound);
    float tXUpperBound = std::max(tXLeftBound, tXRightBound);

    float tYLeftBound = (-0.5 - p.y)/d.y;
    float tYRightBound = (0.5 - p.y)/d.y;
    float tYLowerBound = std::min(tYLeftBound, tYRightBound);
    float tYUpperBound = std::max(tYLeftBound, tYRightBound);

    float tZLeftBound = (-0.5 - p.z)/d.z;
    float tZRightBound = (0.5 - p.z)/d.z;
    float tZLowerBound = std::min(tZLeftBound, tZRightBound);
    float tZUpperBound = std::max(tZLeftBound, tZRightBound);

    float lowerBound = std::max({tXLowerBound, tYLowerBound, tZLowerBound});
    float upperBound = std::min({tXUpperBound, tYUpperBound, tZUpperBound});

    bool intersects = std::max(0.f, lowerBound) < upperBound;

    if (!intersects) return std::make_tuple(intersects, 0.0f, glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0});

    float t = std::max(0.f, lowerBound);
    float x = p.x + d.x * t;
    float y = p.y + d.y * t;
    float z = p.z + d.z * t;

    glm::vec3 norm;
    if (abs(abs(x) - 0.5) < 0.0001) norm = glm::vec3{x / 0.5f, 0, 0};
    else if (abs(abs(y) - 0.5) < 0.0001) norm = glm::vec3{0, y / 0.5f, 0};
    else norm = glm::vec3{0, 0, z / 0.5f};

    return std::make_tuple(intersects, t, norm, glm::vec3{x, y, z});

}

std::tuple<float, float> Cube::unitSquare(glm::vec3 coordinate) {
    if (abs(coordinate.x - 0.5) < 0.0001) return std::make_tuple(1.f - coordinate.z + 0.5, 1.f - coordinate.y + 0.5);
    else if (abs(coordinate.x + 0.5) < 0.0001) return std::make_tuple(coordinate.z + 0.5, 1.f - coordinate.y + 0.5);
    else if (abs(coordinate.y - 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, coordinate.z + 0.5);
    else if (abs(coordinate.y + 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, 1.f - coordinate.z + 0.5);
    else if (abs(coordinate.z - 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, 1.f - coordinate.y + 0.5);
    return std::make_tuple(1.f - coordinate.x + 0.5, 1.f - coordinate.y + 0.5);
}
