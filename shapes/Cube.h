#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include "Rectangle.h"

class Cube : public Shape
{
public:
    Cube();
    Cube(int param1, int param2, int param3);
    ~Cube();


    void tessellate(int param1, int param2, int param3);
    static std::tuple<bool, float, glm::vec3, glm::vec3> intersects(glm::vec3 p, glm::vec3 d);
    static std::tuple<float, float> unitSquare(glm::vec3 coordinate);

private:
    float m_sideLength;
    std::vector<Rectangle> m_faces;
};

#endif // CUBE_H
