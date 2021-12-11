#include "Rectangle.h"

#include <iostream>



//  p1 x p2
//   x x x
//  p4 x p3

<<<<<<< HEAD
Rectangle::Rectangle() {}

=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
Rectangle::Rectangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) :
    m_p1(p1),
    m_p2(p2),
    m_p3(p3),
    m_p4(p4),
    m_split_x(1),
    m_split_y(1)
{

}

Rectangle::~Rectangle()
{
}

void Rectangle::splitBy(int n)
{
    m_split_x = n;
    m_split_y = n;
}

void Rectangle::splitByX(int n)
{
    m_split_x = n;
}

void Rectangle::splitByY(int n)
{
    m_split_y = n;
}



std::vector<Triangle> Rectangle::getTriangles()
{
    std::vector<Triangle> triangles;

    glm::vec3 axis1 = m_p2 - m_p1;
    glm::vec3 interval1 = axis1 / float(m_split_y);

    glm::vec3 axis2 = m_p4 - m_p1;
    glm::vec3 interval2 = axis2 / float(m_split_x);

    for (int i = 0; i < m_split_x; i++)
    {
        for (int j = 0; j < m_split_y; j++)
        {

            glm::vec3 p1 = m_p1 + interval2 * float(i) + interval1 * float(j);
            glm::vec3 p2 = m_p1 + interval2 * float(i) + interval1 * float(j + 1);
            glm::vec3 p3 = m_p1 + interval2 * float(i + 1) + interval1 * float(j + 1);
            glm::vec3 p4 = m_p1 + interval2 * float(i + 1) + interval1 * float(j);

            Triangle triangle1 = Triangle(p1, p4, p2);
            Triangle triangle2 = Triangle(p3, p2, p4);

            triangles.push_back(triangle1);
            triangles.push_back(triangle2);
        }
    }

    return triangles;
}
