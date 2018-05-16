//
// Created by lasagnaphil on 5/11/18.
//

#ifndef GENGINE_BSPTREE_H
#define GENGINE_BSPTREE_H

#include "Mesh.h"
#include "nodes/MeshNode.h"
#include <optional.hpp>

struct Plane {
    glm::vec3 origin;
    glm::vec3 dir;

    inline bool isFront(glm::vec3 pos) {
        return glm::dot(pos - origin, dir) > 0;
    }

    inline int compareWithPoint(glm::vec3 pos) {
        float result = glm::dot(pos - origin, dir);
        if (result > 0) return 1;
        else if (result == 0) return 0;
        else return -1;
    }

    inline glm::vec3 intersection(glm::vec3 l1, glm::vec3 l2) {
        return (glm::dot(dir, origin - l2) * l1 + glm::dot(dir, l1 - origin) * l2) / glm::dot(dir, l1 - l2);
    }

    inline float intersectionRatio(glm::vec3 l1, glm::vec3 l2) {
        return glm::dot(dir, origin - l2) / glm::dot(dir, l1 - l2);
    }
};

class BSPTree {

public:
    using Triangle = std::array<Vertex, 3>;
    struct BSPTriangle {
        MeshNode* node;
        Triangle vertices;
    };
    struct BSPRenderInfo {
        MeshNode* node;
        int size;
    };

private:
    struct BSPNode {
        explicit BSPNode(const std::vector<BSPTriangle>& triangles);

        std::unique_ptr<BSPNode> front;
        std::unique_ptr<BSPNode> back;
        Plane plane;
        BSPTriangle triangle;
    };

public:
    void build(Node* rootNode);

    void sortVertices(glm::vec3 viewLoc);

    void render();

    int countNodes() {
        return countNodes(root.get());
    }

    int countNodes(BSPNode* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->front.get()) + countNodes(node->back.get());
    }

private:
    void addTriangles(std::vector<BSPTriangle>& triangles, Node* node);

    void iterateNodes(glm::vec3 viewLoc, BSPNode* node);

    void addSortedVertices(const BSPTriangle& triangle, bool clear = false);

    std::unique_ptr<BSPNode> root;
    std::vector<Vertex> sortedTriangles;
    std::vector<BSPRenderInfo> renderInfos;
    unsigned int vao, vbo;

    int triangleCount = 0;
};


#endif //GENGINE_BSPTREE_H
