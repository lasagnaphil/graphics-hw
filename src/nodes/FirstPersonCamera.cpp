//
// Created by lasagnaphil on 2/8/18.
//

#include "../Scene.h"
#include "../AppSettings.h"
#include "../InputManager.h"

#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(float yaw, float pitch) :
        Camera(),
        yaw(yaw), pitch(pitch),
        movementSpeed(Settings::Speed),
        mouseSensitivity(Settings::Sensitivity),
        constrainPitch(true)
{
    viewport = {.x = 0, .y = 0, .width = AppSettings::ScreenWidth, .height = AppSettings::ScreenHeight};
    updateCameraVectors();
}

void FirstPersonCamera::update(float dt) {

    //
    // Update based on mouse / keyboard input
    //
    auto inputMgr = InputManager::getInstance();

    // Mouse movement
    if (mouseMovementEnabled) {
        auto mouseOffsetI = inputMgr->relMousePos();
        auto mouseOffset = glm::vec2((float) mouseOffsetI.x, (float) mouseOffsetI.y);

        mouseOffset *= mouseSensitivity;

        yaw += mouseOffset.x;
        pitch += mouseOffset.y;

        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
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
    if (mouseMovementEnabled) {
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

    Camera::update(dt);
}

void FirstPersonCamera::processInput(SDL_Event& ev) {
    if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_m) {
            mouseMovementEnabled = !mouseMovementEnabled;
        }
    }
}

void FirstPersonCamera::updateCameraVectors() {
    glm::quat quatX = glm::angleAxis(-glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat quatY = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    setRotation(glm::normalize(quatX * quatY));
    // rotation = glm::toQuat(glm::orientate3(glm::vec3(0.0f, glm::radians(pitch), -glm::radians(yaw))));
}


