//
// Created by lasagnaphil on 5/11/18.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <optional.hpp>
#include "BSPTree.h"
#include "nodes/Camera.h"

BSPTree::BSPNode::BSPNode(const std::vector<BSPTree::BSPTriangle>& triangles)
        : triangle(triangles[0])
{
    std::vector<BSPTriangle> frontTriangles;
    std::vector<BSPTriangle> backTriangles;
    {
        auto transform = triangle.node->getWorldTransform();
        auto v1 = glm::vec3(transform * glm::vec4(triangle.vertices[0].position, 1.0f));
        auto v2 = glm::vec3(transform * glm::vec4(triangle.vertices[1].position, 1.0f));
        auto v3 = glm::vec3(transform * glm::vec4(triangle.vertices[2].position, 1.0f));
        plane.dir = glm::cross(v2 - v1, v3 - v1);
        plane.origin = (v1 + v2 + v3) / 3.f;
    }
    for (int i = 1; i < triangles.size(); ++i) {
        MeshNode* currentNode = triangles[i].node;
        auto transform = currentNode->getWorldTransform();
        auto v1 = triangles[i].vertices[0];
        auto v2 = triangles[i].vertices[1];
        auto v3 = triangles[i].vertices[2];

        // checks each vertex if it is in front or back
        bool front1 = plane.isFront(glm::vec3(transform * glm::vec4(v1.position, 1.0f)));
        bool front2 = plane.isFront(glm::vec3(transform * glm::vec4(v2.position, 1.0f)));
        bool front3 = plane.isFront(glm::vec3(transform * glm::vec4(v3.position, 1.0f)));

        if (!((front1 && front2 && front3) || (!front1 && !front2 && !front3))) {
            // Plane intersects triangle. Split the triangle with the plane
            bool moreVerticesInFront;
            Vertex p1, p2, p3;
            if (front1 == front2) {
                p1 = v1; p2 = v2; p3 = v3;
                moreVerticesInFront = front1;
            }
            else if (front2 == front3) {
                p1 = v2; p2 = v3; p3 = v1;
                moreVerticesInFront = front2;
            }
            else if (front3 == front1) {
                p1 = v3; p2 = v1; p3 = v2;
                moreVerticesInFront = front3;
            }
            auto p1t = glm::vec3(transform * glm::vec4(p1.position, 1.0f));
            auto p2t = glm::vec3(transform * glm::vec4(p2.position, 1.0f));
            auto p3t = glm::vec3(transform * glm::vec4(p3.position, 1.0f));

            auto t1 = plane.intersectionRatio(p1t, p3t);
            auto m1Pos = t1 * p1.position + (1 - t1) * p3.position;
            auto m1Normal = t1 * p1.normal + (1 - t1) * p3.normal;
            auto m1TexCoords = t1 * p1.texCoords + (1 - t1) * p3.texCoords;
            auto m1 = Vertex { m1Pos, m1Normal, m1TexCoords };

            auto t2 = plane.intersectionRatio(p2t, p3t);
            auto m2Pos = t2 * p2.position + (1 - t2) * p3.position;
            auto m2Normal = t2 * p2.normal + (1 - t2) * p3.normal;
            auto m2TexCoords = t2 * p2.texCoords + (1 - t2) * p3.texCoords;
            auto m2 = Vertex { m2Pos, m2Normal, m2TexCoords };

            if (moreVerticesInFront) {
                frontTriangles.push_back({currentNode, {p1, p2, m1}});
                frontTriangles.push_back({currentNode, {p2, m2, m1}});
                backTriangles.push_back({currentNode, {p3, m1, m2}});
            }
            else {
                backTriangles.push_back({currentNode, {p1, p2, m1}});
                backTriangles.push_back({currentNode, {p2, m2, m1}});
                frontTriangles.push_back({currentNode, {p3, m1, m2}});
            }
        }
        else {
            if (front1) {
                // every vertex is at front
                frontTriangles.push_back({currentNode, {v1, v2, v3}});
            }
            else  {
                // every vertex is at back
                backTriangles.push_back({currentNode, {v1, v2, v3}});
            }
        }
    }

    if (!frontTriangles.empty()) {
        front = std::make_unique<BSPNode>(frontTriangles);
    }
    if (!backTriangles.empty()) {
        back = std::make_unique<BSPNode>(backTriangles);
    }
}

void BSPTree::build(Node* rootNode) {
    std::vector<BSPTriangle> triangles;

    for (auto child : rootNode->getChildren()) {
        addTriangles(triangles, child);
    }

    if (triangles.empty()) { return; }

    std::sort(triangles.begin(), triangles.end(), [](BSPTriangle& t1, BSPTriangle& t2) {
        float area1 = glm::length(glm::cross(
                t1.vertices[2].position - t1.vertices[0].position,
                t1.vertices[1].position - t1.vertices[0].position));
        float area2 = glm::length(glm::cross(
                t2.vertices[2].position - t2.vertices[0].position,
                t2.vertices[1].position - t2.vertices[0].position));
        return area1 > area2;
    });

    root = std::make_unique<BSPNode>(std::move(triangles));
    int triangleCount = countNodes();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * triangleCount * sizeof(Vertex),
                 &sortedTriangles[0], GL_DYNAMIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void BSPTree::sortVertices(glm::vec3 viewLoc) {
    sortedTriangles.clear();
    renderInfos.clear();
    iterateNodes(viewLoc, root.get());
    addSortedVertices({}, false);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sortedTriangles.size() * sizeof(Vertex), &sortedTriangles[0]);
}

void BSPTree::render() {
    int counter = 0;

    glBindVertexArray(vao);
    for (auto& renderInfo : renderInfos) {
        renderInfo.node->updateShaderUniforms();
        renderInfo.node->mesh->bindTexture(renderInfo.node->shader);
        glDrawArrays(GL_TRIANGLES, counter, renderInfo.size);
        counter += renderInfo.size;
    }
    glBindVertexArray(0);
}

void BSPTree::addTriangles(std::vector<BSPTree::BSPTriangle>& triangles, Node* node) {
    if (auto meshNode = node->cast<MeshNode>()) {
        if (meshNode->mesh->isDepthSorted) {
            auto& vertices = meshNode->mesh->vertices;
            for (int i = 0; i < vertices.size() / 3; ++i) {
                triangles.push_back(
                    {meshNode, {vertices[3*i], vertices[3*i+1], vertices[3*i+2]}}
                );
            }
        }
    }

    for (auto child : node->getChildren()) {
        addTriangles(triangles, child);
    }
}

void BSPTree::iterateNodes(glm::vec3 viewLoc, BSPTree::BSPNode* node) {
    if (node == nullptr) { return; }
    // view point is front of plane
    else if (node->plane.compareWithPoint(viewLoc) == 1) {
        iterateNodes(viewLoc, node->back.get());
        addSortedVertices(node->triangle);
        iterateNodes(viewLoc, node->front.get());
    }
    // view point is back of plane
    else if (node->plane.compareWithPoint(viewLoc) == -1) {
        iterateNodes(viewLoc, node->front.get());
        addSortedVertices(node->triangle);
        iterateNodes(viewLoc, node->back.get());
    }
    else {
        iterateNodes(viewLoc, node->front.get());
        iterateNodes(viewLoc, node->back.get());
    }
}

void BSPTree::addSortedVertices(const BSPTriangle& triangle, bool clear) {
    static MeshNode* currentNode = nullptr;
    static int count = 0;
    static std::vector<Vertex> buffer;
    if (currentNode == nullptr) {
        currentNode = triangle.node;
    }
    else if (clear) {
        sortedTriangles.insert(sortedTriangles.end(), buffer.begin(), buffer.end());
        renderInfos.push_back(BSPRenderInfo { currentNode, buffer.size() });
        currentNode = nullptr;
        count = 0;
        buffer.clear();
        return;
    }
    else if (triangle.node != currentNode) {
        sortedTriangles.insert(sortedTriangles.end(), buffer.begin(), buffer.end());
        renderInfos.push_back(BSPRenderInfo { currentNode, buffer.size() });
        currentNode = triangle.node;
        count = 0;
        buffer.clear();
    }
    buffer.push_back(triangle.vertices[0]);
    buffer.push_back(triangle.vertices[1]);
    buffer.push_back(triangle.vertices[2]);
    count++;
}


