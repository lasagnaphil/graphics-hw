//
// Created by lasagnaphil on 2017-03-25.
//

#include <glm/ext.hpp>
#include "MeshNode.h"
MeshNode::MeshNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader) :
        mesh(std::move(mesh)), shader(std::move(shader)) {
}

void MeshNode::update(float dt) {
}

void MeshNode::render() {
    shader->use();
    shader->setMat4("model", worldTransform);
    shader->setFloat("material.shininess", 64.0f);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, material->diffuse.getID());
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, material->specular.getID());

    mesh->draw(shader);
}

void MeshNode::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
}

void MeshNode::setMesh(std::shared_ptr<Mesh> mesh) {
    this->mesh = mesh;
}

MeshNode::BoundingBox MeshNode::findBoundingBox(glm::mat4 projViewMatrix) {
    std::vector<glm::vec3> vertices = getAllVertices();
    std::vector<glm::vec2> screenVertices;

    std::transform(vertices.begin(), vertices.end(), std::back_inserter(screenVertices), [&](glm::vec3 pos) {
        glm::vec4 result = projViewMatrix * glm::vec4(pos, 1.f);
        return glm::vec2(result.x / result.w, result.y / result.w);
    });

    BoundingBox box {
            .leftTop = screenVertices[0],
            .rightBottom = screenVertices[0]
    };

    for (auto& pos : screenVertices) {
        if (pos.x < box.leftTop.x) {
            box.leftTop.x = pos.x;
        } else if (pos.x > box.rightBottom.x) {
            box.rightBottom.x = pos.x;
        }
        if (pos.y < box.leftTop.y) {
            box.leftTop.y = pos.y;
        } else if (pos.y > box.rightBottom.y) {
            box.rightBottom.y = pos.y;
        }
    }
    return box;
}

std::vector<glm::vec3> MeshNode::getAllVertices() {
    std::vector<glm::vec3> vertices;

    auto verticesOfMesh = meshToVertices();
    vertices.insert(vertices.end(), verticesOfMesh.begin(), verticesOfMesh.end());

    flat_transform(children.begin(), children.end(), std::back_inserter(vertices), [&](Node* child) {
        auto meshChild = dynamic_cast<MeshNode*>(child);
        if (meshChild) {
            return meshChild->getAllVertices();
        } else return std::vector<glm::vec3>();
    });

    return vertices;
}

std::vector<glm::vec3> MeshNode::meshToVertices() {
    std::vector<glm::vec3> vertices;
    std::transform(mesh->vertices.begin(), mesh->vertices.end(), std::back_inserter(vertices), [&](Vertex& vertex) {
        auto result = worldTransform * glm::vec4(vertex.position, 1.f);
        return glm::vec3(result);
    });
    return vertices;
}
