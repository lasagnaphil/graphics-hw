//
// Created by lasagnaphil on 2017-03-25.
//

#include "MeshNode.h"

#include <variant>

MeshNode::MeshNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, Shader shader) :
        mesh(std::move(mesh)), material(std::move(material)), shader(shader) {
}

void MeshNode::update(float dt) {
}

void MeshNode::render() {
    shader.use();
    shader.setMat4("model", worldTransform);
    shader.setFloat("material.shininess", material->shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->diffuse.getID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material->specular.getID());

    mesh->draw(shader);
}

void MeshNode::setShader(Shader shader) {
    this->shader = shader;
}
