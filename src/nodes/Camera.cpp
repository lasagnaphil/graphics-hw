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
    rotation.z -= mouseOffset.x;
    //rotation.y += mouseOffset.y;

    // Keyboard movement
    float velocity = movementSpeed * dt;
    if (inputMgr->keyboardPressed(SDLK_w)) {
        position += getFrontVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_s)) {
        position -= getFrontVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_a)) {
        position += getRightVec() * velocity;
    }
    else if (inputMgr->keyboardPressed(SDLK_d)) {
        position -= getRightVec() * velocity;
    }

    // Mouse scroll movement
}

void Camera::processInput(SDL_Event& event) {
    if (event.type == SDL_MOUSEWHEEL) {
        // TODO
    }
}

