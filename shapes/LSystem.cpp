#include "LSystem.h"
#include "Rectangle.h"
#include "Cylinder.h"
#include <iostream>
#include "glm/ext.hpp"

LSystem::LSystem()
{

}

LSystem::LSystem(int param1, glm::mat4 initTransform, std::map<const char, std::string> rules, float angle) :
    m_n(param1),
    m_initTransform(initTransform),
    m_rules(rules),
    m_angle(angle)
{
}

LSystem::~LSystem()
{
}

std::vector<std::tuple<Triangle, glm::mat4>> LSystem::getTriangles(int param1) {
    m_lines.clear();
    std::string init = "X";
    std::string instr = makeInstr(init, param1);
    std::vector<std::tuple<Triangle, glm::mat4>> out;
    float len = 0.1;




    m_transforms.push(m_initTransform);
    for (char const &c: instr) {
        glm::mat4 curr = m_transforms.top();
        m_transforms.pop();
        if (c == 'F') {
            Rectangle r = Rectangle(
                        glm::vec3{-0.01, 0, -0.01},
                        glm::vec3{0.01, 0, 0.01},
                        glm::vec3{0.01, len, 0.01},
                        glm::vec3{-0.01, len, -0.01}
                        );

            Rectangle rO = Rectangle(
                        glm::vec3{-0.01, 0, -0.01},
                        glm::vec3{-0.01, len, -0.01},
                        glm::vec3{0.01, len, 0.01},
                        glm::vec3{0.01, 0, 0.01}
                        );


            m_lines.push_back(std::make_tuple(r,curr));
            m_lines.push_back(std::make_tuple(rO,curr));
            curr *= glm::translate(glm::vec3{0, len, 0});
        } else if (c == '+') {
            curr *= glm::rotate(glm::radians(m_angle), glm::normalize(glm::vec3{-1, 0, 1}));
        } else if (c == '-') {
            curr *= glm::rotate(glm::radians(-m_angle), glm::normalize(glm::vec3{-1, 0, 1}));
        } else if (c == '[') {
            m_transforms.push(curr);
        } else if (c == 'R') {
            if (rand() % 2 == 0) curr = glm::mat4(0);
        }

        if (c != ']') {
            m_transforms.push(curr);
        }
    }
    m_transforms.pop();

    for (int i = 0; i < m_lines.size(); i++)
    {
        Rectangle r;
        glm::mat4 t;


        std::tie(r, t) = m_lines[i];
        r.splitBy(1);

        std::vector<Triangle> tris = r.getTriangles();

        for (int j = 0; j < tris.size(); j++)
        {
            out.push_back(std::make_tuple(tris[j], t));
        }
    }

    std::cout << "finished" << std::endl;

    return out;
}

std::string LSystem::makeInstr(std::string inp, int n) {

    if (n == 0) {
        return inp;
    }

    std::string out = "";

    for (char const &c: inp) {
        if (m_rules.count(c) > 0) {
            out += m_rules.at(c);
        } else {
            out += c;
        }
    }

    return makeInstr(out, n - 1);
}

