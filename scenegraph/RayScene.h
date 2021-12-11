#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "RGBA.h"
#include <unordered_map>
#include "QImage"

#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();

    std::tuple<bool, RGBA> intersect(glm::vec4 p, glm::vec4 d, CS123SceneCameraData camera);
    void printObjects();

private:
    std::tuple<bool, float, glm::vec3, glm::vec3> chooseObject(CS123ScenePrimitive primitive, glm::vec3 p, glm::vec3 d);
    RGBA illuminatePixel(CS123ScenePrimitive primitive, glm::vec4 worldN, glm::vec4 interN, CS123SceneCameraData camera, RGBA texture, float blend, int depth, glm::vec4 p, glm::vec4 d);
    RGBA getTexture(std::tuple<float, float> uv, CS123SceneFileMap textureMap);
    std::unordered_map<std::string,QImage> m_textures;
    bool obstructed(CS123SceneLightData light, glm::vec4 intersection);
    RGBA uvMap(CS123ScenePrimitive primitive, glm::vec3 intersection);
    std::tuple<bool, CS123ScenePrimitive, glm::vec4, glm::vec4, RGBA, float, glm::vec3> reflect(glm::vec3 n, glm::vec3 s, glm::vec3 v);
    int m_recursionDepth;

};

#endif // RAYSCENE_H
