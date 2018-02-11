//
// Created by lasagnaphil on 2/8/18.
//

#include "../Scene.h"
#include "../AppSettings.h"
#include "../InputManager.h"

#include "Camera.h"

Camera::Camera() : Spatial(),
                   shaders(0),
                   movementSpeed(Settings::Speed),
                   mouseSensitivity(Settings::Sensitivity),
                   zoom(Settings::Zoom) {
    viewport = {.x = 0, .y = 0, .width = AppSettings::ScreenWidth, .height = AppSettings::ScreenHeight};
}

void Camera::cameraUpdate() {
    for (auto shader : shaders) {
        shader.use();
        glm::mat4 projection = getPerspectiveMatrix();
        shader.setMat4("proj", projection);
        glm::mat4 view = getViewMatrix();
        shader.setMat4("view", view);
    }
}

void Camera::update(float dt) {
    auto inputMgr = InputManager::getInstance();

    // Mouse movement
    auto mouseOffsetI = inputMgr->relMousePos();
    auto mouseOffset = glm::vec2((float)mouseOffsetI.x, (float)mouseOffsetI.y);

    mouseOffset *= mouseSensitivity;
    printf("%f %f\n", mouseOffset.x, mouseOffset.y);

    rotation.x += mouseOffset.y;
    if (rotation.x > glm::radians(89.0f)) rotation.x = glm::radians(89.0f);
    if (rotation.x < glm::radians(-89.0f)) rotation.x = glm::radians(-89.0f);

    rotation.z -= mouseOffset.x;

    // Keyboard movement
    float velocity = movementSpeed * dt;
    if (inputMgr->keyboardPressed(SDLK_w)) {
        position += getFrontVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_s)) {
        position -= getFrontVec() * velocity;
    }
    if (inputMgr->keyboardPressed(SDLK_a)) {
        position += getRightVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_d)) {
        position -= getRightVec() * velocity;
    }
    if (inputMgr->keyboardPressed(SDLK_q)) {
        position += getUpVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_e)) {
        position -= getUpVec() * velocity;
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
}

void Camera::processInput(SDL_Event& event) {
    if (event.type == SDL_MOUSEWHEEL) {
        // TODO
    }
}

