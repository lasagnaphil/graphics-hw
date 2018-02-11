//
// Created by lasagnaphil on 2017-03-25.
//

#include "MeshNode.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

MeshNode::MeshNode(std::shared_ptr<Mesh> mesh, Shader shader) :
        mesh(std::move(mesh)), shader(shader) {
}

void MeshNode::update(float dt) {
}

void MeshNode::render() {
    shader.use();
    shader.setMat4("model", worldTransform);
    mesh->draw(shader);
}

MeshNode& MeshNode::setShader(Shader shader) {
    this->shader = shader;
    return *this;
}
