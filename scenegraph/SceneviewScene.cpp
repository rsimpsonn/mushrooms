#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/Cube.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"
#include "shapes/Cylinder.h"
#include "shapes/Shape.h"
#include <iostream>
#include "glm/ext.hpp"

using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
    m_cube = std::make_unique<Cube>(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
    m_cone = std::make_unique<Cone>(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
    m_cylinder = std::make_unique<Cylinder>(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
    m_sphere = std::make_unique<Sphere>(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setGlobalData();
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("isShapeScene", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setGlobalData(){
    // [TODO] pass global data to shader.vert using m_phongShader
    m_phongShader->setUniform("ka", m_globalData.ka);
    m_phongShader->setUniform("kd", m_globalData.kd);
    m_phongShader->setUniform("ks", m_globalData.ks);
}


void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
    for (int i = 0; i < m_lights.size(); i++) {
        std::cout << "adding light" << std::endl;
        CS123SceneLightData l = m_lights[i];
        m_phongShader->setLight(l);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    for (int i = 0; i < m_objects.size(); i++) {
        glm::mat4x4 t;
        CS123ScenePrimitive p;
        std::tie(p, t) = m_objects[i];

        std::cout << glm::to_string(t) << std::endl;

        m_phongShader->applyMaterial(p.material);
        m_phongShader->setUniform("m", t);


        switch (p.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            m_cube->draw();
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            m_cone->draw();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            m_cylinder->draw();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            m_sphere->draw();
            break;
        default:
            break;
        }
    }

}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.


    // can just loop thru every object, because the pointers store the previous params and will only retessellate for necessary changes
    for (int i = 0; i < m_objects.size(); i++) {
        glm::mat4x4 t;
        CS123ScenePrimitive p;
        std::tie(p, t) = m_objects[i];
        switch (p.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            m_cube->tessellate(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            m_cone->tessellate(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            m_cylinder->tessellate(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            m_sphere->tessellate(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3);
            break;
        default:
            break;
        }
    }
}

