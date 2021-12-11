#ifndef MYCELIUM_H
#define MYCELIUM_H

#include "shapes/Shape.h"
#include "Triangle.h"
#include <map>



class Mycelium: public Shape
{
public:
    Mycelium();



    Mycelium(int param1, int param2, int param3);
    ~Mycelium();

    void tessellate(int param1, int param2, int param3);
    std::vector<std::tuple<Triangle,glm::mat4>> getTriangles(int parameter1, int parameter2, int parameter3);

private:
    std::map<const char, std::string> m_species;
};

#endif // MYCELIUM_H
