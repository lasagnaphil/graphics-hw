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
        shader.use();
        shader.setMat4("proj", getPerspectiveMatrix());
        shader.setMat4("view", getViewMatrix());
        shader.setVec3("viewPos", getGlobalPosition());
    }
}
