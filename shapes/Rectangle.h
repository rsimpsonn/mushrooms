#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <vector>
#include "shapes/Triangle.h"

class Rectangle
{
public:
<<<<<<< HEAD
    Rectangle();
=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
    Rectangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
    ~Rectangle();
    void splitBy(int n);
    void splitByX(int n);
    void splitByY(int n);

    std::vector<Triangle> getTriangles();

private:
    int m_split_x;
    int m_split_y;

    glm::vec3 m_p1;
    glm::vec3 m_p2;
    glm::vec3 m_p3;
    glm::vec3 m_p4;
};

#endif // RECTANGLE_H
