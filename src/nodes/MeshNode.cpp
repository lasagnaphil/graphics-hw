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
    // if the mesh is depth sorted, draw it later in Scene::render() using BSP Tree
    if (mesh->isDepthSorted) { return; }

    updateShaderUniforms();

    mesh->draw(shader);
}

void MeshNode::updateShaderUniforms() {
    shader->use();
    shader->setMat4("model", worldTransform);
    shader->setVec4("material.ambient", mesh->material->ambient);
    shader->setVec4("material.diffuse", mesh->material->diffuse);
    shader->setVec4("material.specular", mesh->material->ambient);
    shader->setFloat("material.shininess", mesh->material->shininess);
}

void MeshNode::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
}

void MeshNode::setMesh(std::shared_ptr<Mesh> mesh) {
    this->mesh = mesh;
}

MeshNode::BoundingBox2D MeshNode::findBoundingBox(glm::mat4 projViewMatrix) {
    std::vector<glm::vec3> vertices = getAllVertices();
    std::vector<glm::vec2> screenVertices;

    std::transform(vertices.begin(), vertices.end(), std::back_inserter(screenVertices), [&](glm::vec3 pos) {
        glm::vec4 result = projViewMatrix * glm::vec4(pos, 1.f);
        return glm::vec2(result.x / result.w, result.y / result.w);
    });

    BoundingBox2D box {
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

