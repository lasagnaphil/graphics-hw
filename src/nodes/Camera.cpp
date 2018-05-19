//
// Created by lasagnaphil on 4/7/18.
//

#include "Camera.h"
#include "../Scene.h"

void Camera::update(float dt) {
    //
    // Update shader uniforms
    //
    for (auto shader : scene->getShaders()) {
        shader->use();
        shader->setMat4("proj", getPerspectiveMatrix());
        shader->setMat4("view", getViewMatrix());
        shader->setVec3("viewPos", getGlobalPosition());
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(getGlobalPosition(), getGlobalPosition() - getGlobalFrontVec(), getGlobalUpVec());
}

glm::mat4 Camera::getPerspectiveMatrix() const {
    return glm::perspective(
            glm::radians(zoom),
            (float)AppSettings::ScreenWidth / (float)AppSettings::ScreenHeight,
            0.1f,
            100.f
    );
}

