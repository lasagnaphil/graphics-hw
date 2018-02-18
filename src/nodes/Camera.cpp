//
// Created by lasagnaphil on 2/8/18.
//

#include "../Scene.h"
#include "../AppSettings.h"
#include "../InputManager.h"

#include "Camera.h"

Camera::Camera(float yaw, float pitch) :
        Spatial(),
        yaw(yaw), pitch(pitch),
        movementSpeed(Settings::Speed),
        mouseSensitivity(Settings::Sensitivity),
        zoom(Settings::Zoom),
        constrainPitch(true)
{
    viewport = {.x = 0, .y = 0, .width = AppSettings::ScreenWidth, .height = AppSettings::ScreenHeight};
    updateCameraVectors();
}

void Camera::update(float dt) {

    //
    // Update based on mouse / keyboard input
    //
    auto inputMgr = InputManager::getInstance();

    // Mouse movement
    auto mouseOffsetI = inputMgr->relMousePos();
    auto mouseOffset = glm::vec2((float)mouseOffsetI.x, (float)mouseOffsetI.y);

    mouseOffset *= mouseSensitivity;

    yaw += mouseOffset.x;
    pitch += mouseOffset.y;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();

    // Keyboard movement
    float velocity = movementSpeed * dt;
    if (inputMgr->keyboardPressed(SDLK_w)) {
        move(getFrontVec() * velocity);
    }
    else if (inputMgr->keyboardPressed(SDLK_s)) {
        move(-getFrontVec() * velocity);
    }
    if (inputMgr->keyboardPressed(SDLK_a)) {
        move(getRightVec() * velocity);
    }
    else if (inputMgr->keyboardPressed(SDLK_d)) {
        move(-getRightVec() * velocity);
    }
    if (inputMgr->keyboardPressed(SDLK_q)) {
        move(getUpVec() * velocity);
    }
    else if (inputMgr->keyboardPressed(SDLK_e)) {
        move(-getUpVec() * velocity);
    }

    // Mouse scroll movement
    int yoffset = inputMgr->relWheelPos().y;
    if (zoom >= 1.0f && zoom <= 45.0f) {
        zoom -= yoffset;
    }
    if (zoom <= 1.0f) {
        zoom = 1.0f;
    }
    if (zoom >= 45.0f) {
        zoom = 45.0f;
    }

    //
    // Update shader uniforms
    //
    for (auto shader : scene->getShaders()) {
        shader.use();
        shader.setMat4("proj", getPerspectiveMatrix());
        shader.setMat4("view", getViewMatrix());
        shader.setVec3("viewPos", getGlobalPosition());
    }
}

void Camera::updateCameraVectors() {
    glm::quat quatX = glm::angleAxis(-glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat quatY = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    setRotation(glm::normalize(quatX * quatY));
    // rotation = glm::toQuat(glm::orientate3(glm::vec3(0.0f, glm::radians(pitch), -glm::radians(yaw))));
}


