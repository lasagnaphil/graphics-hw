//
// Created by lasagnaphil on 2/18/18.
//

#include "ModelNode.h"

void ModelNode::render() {
    shader.use();
    shader.setMat4("model", worldTransform);
    shader.setFloat("material.shininess", 32.0f);

    model->draw(shader);
}
