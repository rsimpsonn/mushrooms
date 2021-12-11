#ifndef SHAPE_H
#define SHAPE_H

#include "OpenGLShape.h"

class Shape : public OpenGLShape
{
public:
    Shape();
    Shape(int param1, int param2, int param3);
    ~Shape();

    virtual void tessellate(int param1, int param2, int param3);

protected:
    int m_param1;
    int m_param2;
    int m_param3;
    std::vector<GLfloat> m_previousTessellation;
};

#endif // SHAPE_H
