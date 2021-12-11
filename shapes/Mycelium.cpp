#include "Mycelium.h"
#include "LSystem.h"
#include "Rectangle.h"
#include "Cylinder.h"
#include <iostream>
#include "glm/ext.hpp"

Mycelium::Mycelium()
{

}

Mycelium::Mycelium(int param1, int param2, int param3) :
    Shape(param1, param2, param3)
{
    switch (param3) {
    case 1:
        m_species = std::map<const char, std::string>{{'F', "FF"}, {'X', "FFF-[-F[-X]-F[-X]][R-FX]+X"}};
        break;
    case 2:
        m_species = std::map<const char, std::string>{{'F', "FF"}, {'X', "FFF---FY"}, {'Y', "FF-[FY]+Y"}};
        break;
    case 3:
        m_species = std::map<const char, std::string>{{'F', "FF"}, {'X', "FFF-[-F[-X]][-X]+FX"}};
        break;
    default:
        m_species = std::map<const char, std::string>{{'F', "FF"}, {'X', "FFF-[-F[-X]-F[-X]][R-FX]+X"}};
        break;
    }

    initializeOpenGLShapeProperties();

}

Mycelium::~Mycelium()
{
}

void Mycelium::tessellate(int param1, int param2, int param3) {
    param2 = glm::max(3, param2);


    std::vector<GLfloat> tessellation;
    float diff = 2.f * M_PI / param2;
    float curr = 0;


        for (int j = 0; j < 3; j++) {

            //agaric
            LSystem l = LSystem(param1,
                                glm::rotate(curr, glm::vec3{0, 1, 0}) * glm::translate(glm::vec3{0.1, 0, 0}),
                                m_species,
                                25.f);

            std::vector<std::tuple<Triangle,glm::mat4>> tris = l.getTriangles(param1);
            for (int k = 0; k < tris.size(); k++) {
                Triangle t;
                glm::mat4 m;
                std::tie(t, m) = tris[k];

                //std::cout << glm::to_string(m) << std::endl;
                t.addToVector(&tessellation, m);
            }

            LSystem r = LSystem(param1,
                                glm::rotate(glm::radians(180.f), glm::normalize(glm::vec3{1, 0, 1})) * glm::rotate(curr, glm::vec3{0, 1, 0}) * glm::translate(glm::vec3{0, 0, 0.1}),
                                std::map<const char, std::string>{{'F', "FF"}, {'X', "FF[R-X][R+X][FF[-X[+FX]]-F[-X]][R-FX]+X"}},
                                25.f);

            std::vector<std::tuple<Triangle,glm::mat4>> trisR = r.getTriangles(param1);
            for (int k = 0; k < trisR.size(); k++) {
                Triangle t;
                glm::mat4 m;
                std::tie(t, m) = trisR[k];

                //std::cout << glm::to_string(m) << std::endl;
                t.addToVector(&tessellation, m);
            }

        // morels (bell-shaped)



        curr += diff;
    }

    m_vertexData = tessellation;
    initializeOpenGLShapeProperties();

}

std::vector<std::tuple<Triangle,glm::mat4>> Mycelium::getTriangles(int param1, int param2, int param3) {
    param2 = glm::max(3, param2);


    std::vector<std::tuple<Triangle,glm::mat4>> out;
    float diff = 2.f * M_PI / param2;
    float curr = 0;

    for (int i = 0; i < param2; i++) {
            LSystem l = LSystem(param1,
                                glm::rotate(curr, glm::vec3{0, 1, 0}) * glm::translate(glm::vec3{0.1, 0, 0}),
                                m_species,
                                25.f);

            std::vector<std::tuple<Triangle,glm::mat4>> tris = l.getTriangles(param1);
            out.insert(out.end(), tris.begin(), tris.end());

            LSystem r = LSystem(param1,
                                glm::rotate(glm::radians(180.f), glm::normalize(glm::vec3{1, 0, 1})) * glm::rotate(curr, glm::vec3{0, 1, 0}) * glm::translate(glm::vec3{0, 0, 0.1}),
                                std::map<const char, std::string>{{'F', "FF"}, {'X', "FF[R-X][R+X][FF[-X[+FX]]-F[-X]][R-FX]+X"}},
                                25.f);

            std::vector<std::tuple<Triangle,glm::mat4>> trisR = r.getTriangles(param1);
            out.insert(out.end(), trisR.begin(), trisR.end());

        // morels (bell-shaped)



        curr += diff;
    }

    return out;
}
