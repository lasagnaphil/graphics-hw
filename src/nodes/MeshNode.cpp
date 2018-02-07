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
    mesh->draw(shader);
}

MeshNode& MeshNode::setShader(Shader shader) {
    this->shader = shader;
    shader.use();
    uniWorldTransform = glGetUniformLocation(shader.program, "model");
    // uniColor = glGetUniformLocation(shader.program, "objectColor");
    return *this;
}



/*
MeshNode &MeshNode::setColorPerVertex(std::vector<glm::vec4> &colors) {
    colorMode = ColorMode::PerVertex;
    colorPerVertex = colors;
    return *this;
}
 */

