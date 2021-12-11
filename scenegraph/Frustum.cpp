#include "Frustum.h"

std::tuple<float, float, glm::mat4x4> Frustum::generateFrustum(CS123SceneCameraData *camera, int width, int height, float k)
{
    float thetaH = (camera->heightAngle * M_PI) / 180.f;
    float thetaW = 2.0f * atan(tan(thetaH / 2.0f) * (float(width)/height));
    float U = 2.0f * k * tan(thetaW / 2.0f);
    float V = 2.0f * k * tan(thetaH / 2.0f);

    glm::vec3 look = glm::normalize(glm::vec3{camera->look.x, camera->look.y, camera->look.z});
    glm::vec3 up = glm::normalize(glm::vec3{camera->up.x, camera->up.y, camera->up.z});

    glm::vec3 w = glm::normalize(-1.0f * look);
    glm::vec3 v = glm::normalize(up - (glm::dot(up, w) * w));
    glm::vec3 u = glm::cross(v, w);

    glm::mat4x4 mTranslate = {
        1.0f, 0.f, 0.f, -1.0f * camera->pos.x,
        0.f, 1.0f, 0.f, -1.0f * camera->pos.y,
        0.f, 0.f, 1.0f, -1.0f * camera->pos.z,
        0.f, 0.f, 0.f, 1.0f
    };

    glm::mat4x4 mRotate = {
        u.x, u.y, u.z, 0.f,
        v.x, v.y, v.z, 0.f,
        w.x, w.y, w.z, 0.f,
        0.f, 0.f, 0.f, 1.0f
    };

    glm::mat4x4 camT = mTranslate * mRotate;

    glm::mat4x4 invCameraT = glm::inverse(camT);

    return std::make_tuple(U, V, invCameraT);
}
