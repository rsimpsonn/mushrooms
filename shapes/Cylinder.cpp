#include "Cylinder.h"

#include "Circle.h"
#include "Rectangle.h"
#include <iostream>
#include "glm/ext.hpp"

Cylinder::Cylinder()
{

}

Cylinder::Cylinder(int param1, int param2, int param3) :
    Shape(param1, param2, param3),
<<<<<<< HEAD
    m_radius(0.05),
    m_height(0.25)
{

    m_bases = {
        Circle(m_radius, glm::vec3{0, m_radius, 0}, -m_height/2.f),
        Circle(m_radius, glm::vec3{0, -m_radius, 0}, m_height/2.f)
=======
    m_radius(0.5),
    m_height(1)
{

    m_bases = {
        Circle(m_radius, glm::vec3{0, 0.5, 0}, -1),
        Circle(m_radius, glm::vec3{0, -0.5, 0}, 1)
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
    };

    initializeOpenGLShapeProperties();

}

Cylinder::~Cylinder()
{
}

<<<<<<< HEAD


=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
void Cylinder::tessellate(int param1, int param2, int param3) {

    param2 = glm::max(3, param2);

    if (m_previousTessellation.size() > 0 && param1 == m_param1 && param2 == m_param2)
    {
        std::cout << "Getting cylinder from cache" << std::endl;
        m_vertexData = m_previousTessellation;
    } else {
        std::vector<GLfloat> tessellation;

        std::cout << "Redrawing cylinder" << std::endl;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        for (int i = 0; i < m_bases.size(); i++)
        {
            m_bases[i].spokeBy(m_param2);
            m_bases[i].concentricBy(m_param1);

            std::vector<Triangle> tris = m_bases[i].getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                tris[j].addToVector(&tessellation);
            }
        }

        std::vector<glm::vec3> top_vertices = m_bases[0].getOuterVertices();
        std::vector<glm::vec3> bottom_vertices = m_bases[1].getOuterVertices();

        for (int i = 0; i < top_vertices.size(); i++)
        {
            glm::vec3 p1 = top_vertices[i];
            glm::vec3 p2 = bottom_vertices[i];
            glm::vec3 p3 = bottom_vertices[(i + 1) % top_vertices.size()];
            glm::vec3 p4 = top_vertices[(i + 1) % top_vertices.size()];



            Rectangle side = Rectangle(p4, p3, p2, p1);
            side.splitByY(m_param1);
            side.splitByX(1);
            std::vector<Triangle> tris = side.getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                tris[j].remapNormals([](glm::vec3 a) { return glm::normalize(a - glm::vec3{0, a.y, 0}); });
                tris[j].addToVector(&tessellation);
            }
        }

        m_vertexData = tessellation;
        m_previousTessellation = tessellation;
    }

    initializeOpenGLShapeProperties();
}

<<<<<<< HEAD

std::vector<Triangle> Cylinder::getTriangles(int param1, int param2, int param3) {
    param2 = glm::max(3, param2);
        std::vector<Triangle> out;

        std::cout << "Redrawing cylinder" << std::endl;
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        for (int i = 0; i < m_bases.size(); i++)
        {
            m_bases[i].spokeBy(m_param2);
            m_bases[i].concentricBy(m_param1);

            std::vector<Triangle> tris = m_bases[i].getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                out.push_back(tris[j]);
            }
        }

        std::vector<glm::vec3> top_vertices = m_bases[0].getOuterVertices();
        std::vector<glm::vec3> bottom_vertices = m_bases[1].getOuterVertices();

        for (int i = 0; i < top_vertices.size(); i++)
        {
            glm::vec3 p1 = top_vertices[i];
            glm::vec3 p2 = bottom_vertices[i];
            glm::vec3 p3 = bottom_vertices[(i + 1) % top_vertices.size()];
            glm::vec3 p4 = top_vertices[(i + 1) % top_vertices.size()];



            Rectangle side = Rectangle(p4, p3, p2, p1);
            side.splitByY(m_param1);
            side.splitByX(1);
            std::vector<Triangle> tris = side.getTriangles();

            for (int j = 0; j < tris.size(); j++)
            {
                tris[j].remapNormals([](glm::vec3 a) { return glm::normalize(a - glm::vec3{0, a.y, 0}); });
                out.push_back(tris[j]);
            }
        }
       return out;

}

=======
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
std::tuple<bool, float, glm::vec3, glm::vec3> Cylinder::intersects(glm::vec3 p, glm::vec3 d)
{
    float a = pow(d.x, 2) + pow(d.z, 2);
    float b = (2.0f * p.x * d.x) + (2.0f * p.z * d.z);
    float c = -0.25f + pow(p.x, 2) + pow(p.z, 2);
    float dis = pow(b, 2) - (4.0f * a * c);

    bool passesBody = true;

    float t;
    float y;
    float x;
    float z;

    if (dis >= 0) {
        float tNegative = ((-1.0f * b) - sqrt(dis)) / (2.0f*a);
        float negativeY = p.y + d.y * tNegative;
        bool nValid = tNegative >= 0 && negativeY >= -0.5 && negativeY <= 0.5;

        float tPositive = ((-1.0f * b) + sqrt(dis)) / (2.0f*a);
        float positiveY = p.y + d.y * tPositive;
        bool pValid = tPositive >= 0 && positiveY >= -0.5 && positiveY <= 0.5;

        if (nValid && pValid) {
            t = std::min(tPositive, tNegative);
            y = p.y + d.y * t;
            x = p.x + d.x * t;
            z = p.z + d.z * t;
        } else if (pValid) {
            t = tPositive;
            y = p.y + d.y * t;
            x = p.x + d.x * t;
            z = p.z + d.z * t;
        } else if (nValid) {
            t = tNegative;
            y = p.y + d.y * t;
            x = p.x + d.x * t;
            z = p.z + d.z * t;
        } else {
            passesBody = false;
        }
    } else {
        passesBody = false;
    }

    bool passesFirstCap = false;
    float cap1T = (0.5 - p.y)/d.y;
    float cap1X = p.x + d.x * cap1T;
    float cap1Z = p.z + d.z * cap1T;

    if (cap1T >= 0 && (pow(cap1X, 2) + pow(cap1Z, 2)) <= 0.25f) {
        passesFirstCap = true;
    }

    bool passesSecondCap = false;
    float cap2T = (-0.5 - p.y)/d.y;
    float cap2X = p.x + d.x * cap2T;
    float cap2Z = p.z + d.z * cap2T;

    if (cap2T >= 0 && (pow(cap2X, 2) + pow(cap2Z, 2)) <= 0.25f) {
        passesSecondCap = true;
    }

    if (passesBody && passesFirstCap && passesSecondCap) {
        if (t < cap1T && t < cap2T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0, z}), glm::vec3{x, y, z});
        } else if (cap1T < cap2T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, 1, 0}), glm::vec3{cap1X, 0.5, cap1Z});
        } else {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X, -0.5, cap2Z});
        }
    } else if (passesBody && passesFirstCap) {
        if (t < cap1T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0, z}), glm::vec3{x, y, z});
        } else {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, 1, 0}), glm::vec3{cap1X, 0.5, cap1Z});
        }
    } else if (passesBody && passesSecondCap) {
        if (t < cap2T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0, z}), glm::vec3{x, y, z});
        } else {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X, -0.5, cap2Z});
        }
    } else if (passesFirstCap && passesSecondCap) {
        if (cap1T < cap2T) {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, 1, 0}), glm::vec3{cap1X, 0.5, cap1Z});
        } else {
            return std::make_tuple(true, t, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X, -0.5, cap2Z});
        }
    } else if (passesBody) {
        return std::make_tuple(true, t, glm::normalize(glm::vec3{x, 0, z}), glm::vec3{x, y, z});
    } else if (passesFirstCap) {
        return std::make_tuple(true, t, glm::normalize(glm::vec3{0, 1, 0}), glm::vec3{cap1X, 0.5, cap1Z});
    } else if (passesSecondCap) {
        return std::make_tuple(true, t, glm::normalize(glm::vec3{0, -1, 0}), glm::vec3{cap2X, -0.5, cap2Z});
    }

    return std::make_tuple(false, 0, glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0});

}

std::tuple<float, float> Cylinder::unitSquare(glm::vec3 coordinate) {
    if (abs(coordinate.y - 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, coordinate.z + 0.5);
    else if (abs(coordinate.y + 0.5) < 0.0001) return std::make_tuple(coordinate.x + 0.5, 1.f - coordinate.z + 0.5);
    // on body

    float theta;

    if (coordinate.x > 0 && coordinate.z < 0) {
        theta = acos((coordinate.x * 0.5 + coordinate.z * 0) / 0.25);
    } else if (coordinate.x < 0 && coordinate.z < 0) {
        theta = float(M_PI) / 2.f + acos((coordinate.x * 0 + coordinate.z * -0.5) / 0.25);
    } else if (coordinate.x < 0 && coordinate.z > 0) {
        theta = float(M_PI) + acos((coordinate.x * -0.5 + coordinate.z * 0) / 0.25);
    } else {
        theta = 3 * float(M_PI) / 2.f + acos((coordinate.x * 0 + coordinate.z * 0.5) / 0.25);
    }

    return std::make_tuple(theta / (2.f * M_PI), 1.f - coordinate.y + 0.5);

}
