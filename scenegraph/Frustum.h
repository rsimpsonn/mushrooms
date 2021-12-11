#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <tuple>
#include "glm.hpp"
#include "CS123SceneData.h"


class Frustum
{
public:
    static std::tuple<float, float, glm::mat4x4> generateFrustum(CS123SceneCameraData *camera, int width, int height, float k);
};

#endif // FRUSTUM_H
