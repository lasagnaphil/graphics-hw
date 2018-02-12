//
// Created by lasagnaphil on 2/13/18.
//

#include "LightNode.h"
#include "../Scene.h"

void LightNode::update(float dt) {
    for (Shader shader : scene->getShaders()) {
        shader.use();
        shader.setVec3("lightPos", position);
        shader.setVec3("lightColor", color);
    }
}
