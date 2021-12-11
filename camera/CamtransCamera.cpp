/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include <iostream>
#include "glm/ext.hpp"

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "constructor" << std::endl;
    setClip(1.f, 30.f);

    setAspectRatio(1.f);
    setHeightAngle(60.f);
    orientLook(
                    glm::vec4(2.f, 2.f, 2.f, 1.f),
                    glm::vec4(-1.f, -1.f, -1.f, 0.f),
                    glm::vec4(0.f, 1.f, 0.f, 0.f)
    );

}

void CamtransCamera::setAspectRatio(float a)
{
    std::cout << "aspect" << std::endl;
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
    m_thetaW = 2.0f * atan(m_aspectRatio * tan(m_thetaH / 2.f));
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...

    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return  m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return -1.f * m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "orient look" << std::endl;

    m_eye = eye;
    m_w = glm::vec4{glm::normalize(-look.xyz()), 0.f};
    m_up = up;
    m_v = glm::vec4{glm::normalize((m_up - glm::dot(m_up, m_w) * m_w).xyz()), 0.f};
    m_u = glm::vec4{glm::cross(m_v.xyz(), m_w.xyz()), 0.f};

//    std::cout << "Orient look: " << std::endl;
//    std::cout << "Eye: " << glm::to_string(eye) << " m_eye: " << glm::to_string(m_eye) << std::endl;
//    std::cout << "Look: " << glm::to_string(look) << " m_look: " << glm::to_string(-m_w) << std::endl;
//    std::cout << "u: " << glm::to_string(m_u) << std::endl;
//    std::cout << "v: " << glm::to_string(m_v) << std::endl;
//    std::cout << "w: " << glm::to_string(m_w) << std::endl;

//    std::cout << "Up: " << glm::to_string(m_up) << std::endl;
    updateViewMatrix();
    updateProjectionMatrix();
//    std::cout << "View matrix: " << glm::to_string(getViewMatrix()) << std::endl;
//    std::cout << "near: " << m_near << std::endl;
//    std::cout << "far: " << m_far << std::endl;
//    std::cout << "scale matrix: " << glm::to_string(getScaleMatrix()) << std::endl;
//    std::cout << "thetah: " << glm::to_string(m_thetaH) << std::endl;
//    std::cout << "thetaw: " << glm::to_string(m_thetaW) << std::endl;
//    std::cout << "perspective matrix: " << glm::to_string(getPerspectiveMatrix()) << std::endl;
//    std::cout << "Projection matrix: " << glm::to_string(getProjectionMatrix()) << std::endl;
}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "set height" << std::endl;
    m_thetaH = glm::radians(h);
    m_thetaW = 2.0f * atan(m_aspectRatio * tan(m_thetaH / 2.f));
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "translate" << std::endl;
    m_eye += v;
    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in..

    std::cout << "rotate u" << std::endl;
    float rad = glm::radians(degrees);

    glm::mat4x4 rotateX = glm::mat4x4{
            1, 0, 0, 0,
            0, cos(rad), -sin(rad), 0,
            0, sin(rad), cos(rad), 0,
            0, 0, 0, 1};

    glm::vec4 rotatedY = rotateX * glm::vec4{0, 1, 0, 0};
    glm::vec4 rotatedZ = rotateX * glm::vec4{0, 0, 1, 0};
    glm::mat4 invV = glm::inverse(getViewMatrix());
    m_v = invV * rotatedY;
    m_w = invV * rotatedZ;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "rotate v" << std::endl;
    float rad = glm::radians(degrees);

    glm::mat4x4 rotateY = glm::mat4x4{
            cos(rad), 0, sin(rad), 0,
            0, 1, 0, 0,
            -sin(rad), 0, cos(rad), 0,
            0, 0, 0, 1};

    glm::vec4 rotatedX = rotateY * glm::vec4{1, 0, 0, 0};
    glm::vec4 rotatedZ = rotateY * glm::vec4{0, 0, 1, 0};
    glm::mat4 invV = glm::inverse(getViewMatrix());
    m_u = invV * rotatedX;
    m_w = invV * rotatedZ;
    updateViewMatrix();

}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...

    std::cout << "rotate w" << std::endl;

    float rad = glm::radians(degrees);

    glm::mat4x4 rotateZ = glm::mat4x4{
            cos(rad), -sin(rad), 0, 0,
            sin(rad), cos(rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

    glm::vec4 rotatedX = rotateZ * glm::vec4{1, 0, 0, 0};
    glm::vec4 rotatedY = rotateZ * glm::vec4{0, 1, 0, 0};
    glm::mat4 invV = glm::inverse(getViewMatrix());
    m_v = invV * rotatedY;
    m_u = invV * rotatedX;
    updateViewMatrix();

}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in..

    std::cout << "set clip" << std::endl;
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix() {

    std::cout << "update projection" << std::endl;
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {

    std::cout << "update perspective" << std::endl;
    float c = -m_near / m_far;
    m_perspectiveTransformation = glm::transpose(glm::mat4x4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, -1.f / (1.f + c), c / (1.f + c),
            0, 0, -1, 0});
}

void CamtransCamera::updateScaleMatrix() {

    std::cout << "update scale" << std::endl;
    m_scaleMatrix = glm::transpose(glm::mat4x4{
            1.f / (m_far * tan(m_thetaW / 2.f)), 0, 0, 0,
            0, 1.f / (m_far * tan(m_thetaH / 2.f)), 0, 0,
            0, 0, 1.f / m_far, 0,
            0, 0, 0, 1});
}
void CamtransCamera::updateViewMatrix() {

    std::cout << "update view" << std::endl;
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix() {

    std::cout << "update rotation" << std::endl;
    m_rotationMatrix = glm::transpose(glm::mat4x4{
            m_u.x, m_u.y, m_u.z, 0,
            m_v.x, m_v.y, m_v.z, 0,
            m_w.x, m_w.y, m_w.z, 0,
            0, 0, 0, 1});
}

void CamtransCamera::updateTranslationMatrix() {
    std::cout << "update translation" << std::endl;
    m_translationMatrix = glm::transpose(glm::mat4x4{
            1, 0, 0, -m_eye.x,
            0, 1, 0, -m_eye.y,
            0, 0, 1, -m_eye.z,
            0, 0, 0, 1});
}



