#include "SuperShroom.h"
#include "Mycelium.h"
#include "Rectangle.h"
#include "Mushroom.h"
#include "Shape.h"
#include "glm/ext.hpp"

SuperShroom::SuperShroom()
{
}

SuperShroom::SuperShroom(int param1, int param2, int param3, int mushroomType) :
    Shape(param1, param2, param3),
    m_mushroomType(mushroomType)
{
    m_mushroom = std::make_unique<Mushroom>(param1, param2, mushroomType);
    m_mycelium = std::make_unique<Mycelium>(param1, param2, mushroomType);
    initializeOpenGLShapeProperties();
}


void SuperShroom::tessellate(int param1, int param2, int param3) {
    std::vector<Triangle> mushTris = m_mushroom->getTriangles(1, param2, param3);
    std::vector<std::tuple<Triangle, glm::mat4>> mycTris = m_mycelium->getTriangles(param1, param2, param3);
    std::vector<GLfloat> tessellation;


    for (int i = 0; i < mushTris.size(); i++) {
        mushTris[i].addToVector(&tessellation, glm::scale(glm::vec3{param1, param1, param1}));
    }

    for (int i = 0; i < mycTris.size(); i++) {
        Triangle t;
        glm::mat4 m;
        std::tie(t, m) = mycTris[i];

        //std::cout << glm::to_string(m) << std::endl;
        t.addToVector(&tessellation, m);
    }


    m_vertexData = tessellation;

    initializeOpenGLShapeProperties();
}
