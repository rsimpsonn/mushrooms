#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"
#include "CS123XmlSceneParser.h"
#include "glm/gtx/transform.hpp"
#include <iostream>
#include "glm/ext.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    setGlobal(scene.m_globalData);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

   for (int i = 0; i < scene.m_lights.size(); i++)
   {
       addLight(scene.m_lights[i]);
   }

   for (int i = 0; i < scene.m_objects.size(); i++)
   {
       CS123ScenePrimitive p;
       glm::mat4x4 mat;
       std::tie(p, mat) = scene.m_objects[i];
       addPrimitive(p, mat);
   }

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);
    sceneToFill->setGlobal(globalData);

    for (int i = 0; i < parser->getNumLights(); i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        sceneToFill->addLight(lightData);
    }


    CS123SceneNode *root = parser->getRootNode();
    std::vector<std::tuple<CS123SceneNode*, glm::mat4x4>> queue = {std::make_tuple(root, glm::mat4(1.0f))};
    while (queue.size() > 0) {
        std::tuple<CS123SceneNode*, glm::mat4x4> viewing = queue.back();
        queue.pop_back();

        CS123SceneNode* node;
        glm::mat4x4 mat;
        std::tie(node, mat) = viewing;

        for (int i = 0; i < node->transformations.size(); i++)
        {
            switch (node->transformations[i]->type)
            {
            case TransformationType::TRANSFORMATION_MATRIX:
                mat *= node->transformations[i]->matrix;
                break;
            case TRANSFORMATION_ROTATE:
                mat *= glm::rotate(node->transformations[i]->angle, node->transformations[i]->rotate);
                break;
            case TRANSFORMATION_SCALE:
                mat *= glm::scale(node->transformations[i]->scale);
                break;
            case TRANSFORMATION_TRANSLATE:
                mat *= glm::translate(node->transformations[i]->translate);
                break;
            }
        }

        for (int i = 0; i < node->primitives.size(); i++)
        {
            sceneToFill->addPrimitive(*node->primitives[i], mat);
        }

        for (int i = 0; i < node->children.size(); i++)
        {
            queue.insert(queue.begin(), std::make_tuple(node->children[i], mat));
        }
    }

    for (int i = 0; i < sceneToFill->m_objects.size(); i++)
    {
        CS123ScenePrimitive p;
        glm::mat4x4 m;
        std::tie(p, m) = sceneToFill->m_objects[i];
    }
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_objects.push_back(std::make_tuple(scenePrimitive, matrix));
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_globalData = global;
}

