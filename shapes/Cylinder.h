#ifndef CYLINDER_H
#define CYLINDER_H

#include "shapes/Shape.h"
#include "shapes/Circle.h"


class Cylinder: public Shape
{
public:
    Cylinder();
    Cylinder(int param1, int param2, int param3);
    ~Cylinder();

    void tessellate(int param1, int param2, int param3);
    static std::tuple<bool, float, glm::vec3, glm::vec3> intersects(glm::vec3 p, glm::vec3 d);
    static std::tuple<float, float> unitSquare(glm::vec3 coordinate);
<<<<<<< HEAD
    std::vector<Triangle> getTriangles(int param1, int param2, int param3);
=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3

private:
    float m_height;
    float m_radius;
    std::vector<Circle> m_bases;
};

#endif // CYLINDER_H
