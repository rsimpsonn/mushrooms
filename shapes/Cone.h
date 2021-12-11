#ifndef CONE_H
#define CONE_H

#include "Shape.h"
#include "Circle.h"

class Cone : public Shape
{
public:
    Cone();
    Cone(int param1, int param2, int param3);
    ~Cone();

    void tessellate(int param1, int param2, int param3);
    static std::tuple<bool, float, glm::vec3, glm::vec3> intersects(glm::vec3 p, glm::vec3 d);
    static std::tuple<float, float> unitSquare(glm::vec3 coordinate);

private:
    float m_radius;
    float m_height;
    std::vector<Circle> m_faces;
};

#endif // CONE_H
