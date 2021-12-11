#ifndef SUPERSHROOM_H
#define SUPERSHROOM_H

#include "Shape.h"
#include "Mushroom.h"
#include "Mycelium.h"

class SuperShroom : public Shape
{
public:

    SuperShroom(int param1, int param2, int param3, int mushroomType);

    SuperShroom();
    void tessellate(int param1, int param2, int param3);

private:
    int m_mushroomType;
    std::unique_ptr<Mushroom> m_mushroom;
    std::unique_ptr<Mycelium> m_mycelium;
};

#endif // SUPERSHROOM_H
