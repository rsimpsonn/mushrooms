#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <stack>
#include "shapes/Rectangle.h"
#include "shapes/Cylinder.h"
#include "shapes/Triangle.h"
#include <map>


class LSystem
{
public:
    LSystem();
    LSystem(int param1, glm::mat4 initTransform, std::map<const char, std::string> rules, float angle);
    ~LSystem();

    std::vector<std::tuple<Triangle, glm::mat4>> getTriangles(int param1);
    std::string makeInstr(std::string inp, int n);
private:
    std::stack<glm::mat4> m_transforms;
    std::vector<std::tuple<Rectangle,glm::mat4>> m_lines;
    std::map<const char, std::string> m_rules;
    float m_angle;
    int m_n;
    glm::mat4 m_initTransform;
};

#endif // LSYSTEM_H
