#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Shape.h"
#include "CS123SceneData.h"
#include "Circle.h"

class Mushroom : public Shape
{
public:
    Mushroom(int parameter1, int parameter2, int mushroomType);
    ~Mushroom();
    void tessellate(int param1, int param2, int param3);
    std::vector<Triangle> getTriangles(int parameter1, int parameter2, int parameter3);

private:
    void makeUmbrellaShroom(int p1, int p2);
    void makeBellShroom(int p1, int p2);
    void makePancakeShroom(int p1, int p2);

    std::vector<Circle> m_topCircles;
    std::vector<Circle> m_stemCircles;

};


#endif // MUSHROOM_H
