#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Circle.h"


class Sphere : public Shape
{
public:
    Sphere();
    Sphere(int param1, int param2, int param3);
    ~Sphere();

    void tessellate(int param1, int param2, int param3);
    static std::tuple<bool, float, glm::vec3, glm::vec3> intersects(glm::vec3 p, glm::vec3 d);
    static std::tuple<float, float> unitSquare(glm::vec3 coordinate);

private:
    float m_radius;
    std::vector<Circle> m_lats;
    void setLats();
};

#endif // SPHERE_H
