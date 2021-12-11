#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "shapes/Cube.h"
#include "shapes/Sphere.h"
#include "shapes/Cone.h"
#include "shapes/Cylinder.h"
#include "QImage"
#include "QRgb"

#include <iostream>
#include "glm/ext.hpp"


RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    m_recursionDepth = 2;
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
}

RayScene::~RayScene()
{
}

void RayScene::printObjects()
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        glm::mat4x4 oTransformation;
        CS123ScenePrimitive primitive;
        std::tie(primitive, oTransformation) = m_objects[i];

        if (primitive.type == PrimitiveType::PRIMITIVE_CONE) std::cout << "cone" << std::endl;
        if (primitive.type == PrimitiveType::PRIMITIVE_CUBE) std::cout << "cube" << std::endl;
        if (primitive.type == PrimitiveType::PRIMITIVE_SPHERE) std::cout << "sphere" << std::endl;
        if (primitive.type == PrimitiveType::PRIMITIVE_CYLINDER) std::cout << "cylinder" << std::endl;

        std::cout << glm::to_string(oTransformation) << std::endl;
    }
}

std::tuple<bool, float, glm::vec3, glm::vec3> RayScene::chooseObject(CS123ScenePrimitive primitive, glm::vec3 p, glm::vec3 d) {
    switch (primitive.type)
    {
    case PrimitiveType::PRIMITIVE_CUBE:
        return Cube::intersects(p, d);
    case PrimitiveType::PRIMITIVE_CONE:
        return Cone::intersects(p, d);
    case PrimitiveType::PRIMITIVE_CYLINDER:
        return Cylinder::intersects(p, d);
    case PrimitiveType::PRIMITIVE_SPHERE:
        return Sphere::intersects(p, d);
    }
}

bool RayScene::obstructed(CS123SceneLightData light, glm::vec4 intersection) {
    glm::vec3 d = glm::normalize(light.type == LightType::LIGHT_POINT ? light.pos.xyz() - intersection.xyz() : -1.f * light.dir.xyz());
    glm::vec3 p = intersection.xyz() + 0.1f * d;


    for (int i = 0; i < m_objects.size(); i++) {
            CS123ScenePrimitive oL;
            glm::mat4x4 mL;

            std::tie(oL, mL) = m_objects[i];

            glm::mat4x4 invT = glm::inverse(mL);
            glm::vec4 pO = invT * glm::vec4{p, 1.f};
            glm::vec4 dO = invT * glm::vec4{d, 0};

            bool intersects = std::get<0>(chooseObject(oL, pO.xyz(), glm::normalize(dO.xyz())));

            if (intersects) return true;
    }

    return false;

}

std::tuple<bool, CS123ScenePrimitive, glm::vec4, glm::vec4, RGBA, float, glm::vec3> RayScene::reflect(glm::vec3 n, glm::vec3 s, glm::vec3 v) {

    glm::vec3 r = glm::normalize(2.f * n * glm::dot(n, v) - v);

    glm::vec3 d = r;
    glm::vec3 p = s + 0.1f * d;

    float minT = INT_MAX;
    glm::vec3 rNorm;
    glm::vec3 rInter;
    RGBA rText;
    CS123ScenePrimitive rPrim;
    float rBlend;

    bool found = false;

    for (int i = 0; i < m_objects.size(); i++) {
            CS123ScenePrimitive oL;
            glm::mat4x4 mL;

            std::tie(oL, mL) = m_objects[i];

            glm::mat4x4 invT = glm::inverse(mL);
            glm::vec4 pO = invT * glm::vec4{p, 1.f};
            glm::vec4 dO = invT * glm::vec4{d, 0};

            bool intersects;
            float t;
            glm::vec3 interR;
            glm::vec3 normR;
            std::tie(intersects, t, normR, interR) = chooseObject(oL, pO.xyz(), glm::normalize(dO.xyz()));

            if (intersects && t < minT) {
                found = true;
                rNorm = (mL * glm::vec4{normR, 0}).xyz();
                rInter = (mL * glm::vec4{interR, 1}).xyz();
                rPrim = oL;
                rText = uvMap(rPrim, interR);
                rBlend = rPrim.material.blend;
                minT = t;
            }
    }

    return std::make_tuple(found, rPrim, glm::vec4{rInter, 1}, glm::vec4{glm::normalize(rNorm), 0}, rText, rBlend, -r);
}

RGBA RayScene::illuminatePixel(
        CS123ScenePrimitive primitive,
        glm::vec4 worldN,
        glm::vec4 interN,
        CS123SceneCameraData camera,
        RGBA texture,
        float blend,
        int depth,
        glm::vec4 p,
        glm::vec4 d) {
    RGBA color;
    color.a = 255;

    float lightSumR = 0;
    float lightSumG = 0;
    float lightSumB = 0;

    float textR = float(texture.r) / 255.f;
    float textG = float(texture.g) / 255.f;
    float textB = float(texture.b) / 255.f;

    for (int j = 0; j < m_lights.size(); j++) {
        if ((m_lights[j].type == LightType::LIGHT_POINT || m_lights[j].type == LightType::LIGHT_DIRECTIONAL) && !obstructed(m_lights[j], interN)) {
            glm::vec4 L = m_lights[j].type == LightType::LIGHT_POINT ?
                        glm::normalize(m_lights[j].pos - interN) :
                        glm::normalize(-m_lights[j].dir);
            glm::vec4 rI = -L - 2.f * glm::dot(-L, worldN) * worldN;
            glm::vec4 R = glm::normalize(glm::vec4{rI.xyz(), 0.f});
            glm::vec4 V = glm::normalize(p - interN);
            float d = glm::length(m_lights[j].pos - interN);

            float attenuation = m_lights[j].type ==
                    LightType::LIGHT_POINT ?
                        glm::min(1.f, 1.f /
                                 float(m_lights[j].function.x + d * m_lights[j].function.y + pow(d, 2) * m_lights[j].function.z
                                       )) : 1;

            float rDiffuseDot = (blend * textR + (1.f - blend) * m_globalData.kd * primitive.material.cDiffuse.r) * glm::dot(worldN, L);
            float rSpecularDot = m_globalData.ks * primitive.material.cSpecular.r * pow(glm::dot(R, V), primitive.material.shininess);
            if (rSpecularDot + rDiffuseDot > 0) lightSumR += attenuation * m_lights[j].color.r * (rSpecularDot + rDiffuseDot);

            float gDiffuseDot = (blend * textG + (1.f - blend) * m_globalData.kd * primitive.material.cDiffuse.g) * glm::dot(worldN, L);
            float gSpecularDot = m_globalData.ks * primitive.material.cSpecular.g * pow(glm::dot(R, V), primitive.material.shininess);
            if (gSpecularDot + gDiffuseDot > 0) lightSumG += attenuation * m_lights[j].color.g * (gSpecularDot + gDiffuseDot);

            float bDiffuseDot = (blend * textB + (1.f - blend) * m_globalData.kd * primitive.material.cDiffuse.b) * glm::dot(worldN, L);
            float bSpecularDot = m_globalData.ks * primitive.material.cSpecular.b * pow(glm::dot(R, V), primitive.material.shininess);
            if (bSpecularDot + bDiffuseDot > 0) lightSumB += attenuation * m_lights[j].color.b * (bSpecularDot + bDiffuseDot);
        }
    }

    RGBA reflected = RGBA(0, 0, 0, 255);
    if (depth > 0 &&
            m_globalData.ks > 0 &&
            (primitive.material.cReflective.r > 0 ||
             primitive.material.cReflective.g > 0 ||
             primitive.material.cReflective.b > 0)
            ) {
        bool found;
        CS123ScenePrimitive rPrim;
        glm::vec4 rInter;
        glm::vec4 rNorm;
        RGBA rText;
        float rBlend;
        glm::vec3 rRay;

        std::tie(found, rPrim, rInter, rNorm, rText, rBlend, rRay) = reflect(worldN.xyz(), interN.xyz(), d.xyz());
        if (found) {
            reflected = illuminatePixel(rPrim, rNorm, rInter, camera, rText, rBlend, depth - 1, interN, glm::vec4{rRay, 0.f});
            //std::cout << float(reflected.r) << ", " << float(reflected.g) << ", " << float(reflected.b) << std::endl;
         }
    }
    color.r = glm::min(255.f, 255.f * (m_globalData.ka*primitive.material.cAmbient.r + lightSumR + m_globalData.ks*primitive.material.cReflective.r*(float(reflected.r) / 255.f)));
    color.g = glm::min(255.f, 255.f * (m_globalData.ka*primitive.material.cAmbient.g + lightSumG + m_globalData.ks*primitive.material.cReflective.g*(float(reflected.g) / 255.f)));
    color.b = glm::min(255.f, 255.f * (m_globalData.ka*primitive.material.cAmbient.b + lightSumB + m_globalData.ks*primitive.material.cReflective.b*(float(reflected.b) / 255.f)));

    return color;
}

std::tuple<bool, RGBA> RayScene::intersect(glm::vec4 p, glm::vec4 d, CS123SceneCameraData camera)
{

    bool objectIntersects = false;
    float minT = INT_MAX;
    RGBA tPixel = RGBA(0, 0, 0, 255);

    for (int i = 0; i < m_objects.size(); i++)
    {

        glm::mat4x4 oTransformation;
        CS123ScenePrimitive primitive;
        std::tie(primitive, oTransformation) = m_objects[i];


        glm::mat4x4 invT = glm::inverse(oTransformation);

        glm::vec4 pObjectSpace = invT * p;
        glm::vec4 dObjectSpace = invT * d;
        glm::vec3 dO = glm::vec3{dObjectSpace.x, dObjectSpace.y, dObjectSpace.z};

        bool intersects = false;
        float t;
        glm::vec3 n;
        glm::vec3 intersection;

        std::tie(intersects, t, n, intersection) = chooseObject(primitive, glm::vec3{pObjectSpace.x, pObjectSpace.y, pObjectSpace.z}, dO);

        if (!intersects || t > minT) continue;

        glm::vec4 worldN = oTransformation * glm::vec4{n, 0.0f};
        worldN = glm::vec4{glm::normalize(glm::vec3{worldN.x, worldN.y, worldN.z}), 1.0f};
        glm::vec4 interN = oTransformation * glm::vec4{intersection, 1.0f};

//        color.r = (255.f * ((worldN.x + 1.0f) / 2));
//        color.g = (255.f * ((worldN.y + 1.0f) / 2));
//        color.b = (255.f * ((worldN.z + 1.0f) / 2));

        objectIntersects = true;
        minT = t;
        tPixel = illuminatePixel(primitive, worldN, interN, camera, uvMap(primitive, intersection), primitive.material.blend, m_recursionDepth, camera.pos, -d);
    }

    return std::make_tuple(objectIntersects, tPixel);
}

RGBA RayScene::uvMap(CS123ScenePrimitive primitive, glm::vec3 intersection) {
    std::tuple<float, float> uv;
    switch (primitive.type)
    {
    case PrimitiveType::PRIMITIVE_CUBE:
        uv =  Cube::unitSquare(intersection);
        break;
    case PrimitiveType::PRIMITIVE_CONE:
        uv =  Cone::unitSquare(intersection);
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        uv =  Cylinder::unitSquare(intersection);
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        uv =  Sphere::unitSquare(intersection);
        break;
    }
    return getTexture(uv, primitive.material.textureMap);
}

RGBA RayScene::getTexture(std::tuple<float, float> uv, CS123SceneFileMap textureMap) {
    if (!textureMap.isUsed) return RGBA(0, 0, 0, 255);
    float u;
    float v;
    std::tie(u, v) = uv;

    if (m_textures.find(textureMap.filename) == m_textures.end()) {
        QString filename = QString::fromStdString(textureMap.filename);
        QImage img;
        img.load(filename);
        m_textures.insert(std::pair<std::string,QImage>(textureMap.filename, img));
    }

    QImage img = m_textures[textureMap.filename];
    int s = ((int) (u * textureMap.repeatU * img.width())) % img.width();
    int t = ((int) (v * textureMap.repeatV * img.height())) % img.height();

    QColor pixel = QColor(img.pixel(s, t));
    return RGBA(pixel.red(), pixel.green(), pixel.blue(), 255.f);
}



