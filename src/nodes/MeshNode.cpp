//
// Created by lasagnaphil on 2017-03-25.
//

#include "MeshNode.h"

MeshNode::MeshNode(std::shared_ptr<Mesh> mesh, Shader shader) :
        mesh(std::move(mesh)), shader(shader) {
}

void MeshNode::update(float dt) {
}

void MeshNode::render() {
    shader.use();
    shader.setMat4("model", worldTransform);
    shader.setFloat("material.shininess", 64.0f);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, material->diffuse.getID());
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, material->specular.getID());

    mesh->draw(shader);
}

void MeshNode::setShader(Shader shader) {
    this->shader = shader;
}
