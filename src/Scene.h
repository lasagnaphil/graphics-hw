#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include <stack>
#include <functional>
#include <iostream>
#include "nodes/Node.h"
#include "nodes/Spatial.h"

class Scene {
public:
    Scene() : rootNode() {
    }

    Node& getRootNode() { return rootNode; }

    void render() {
        render(&rootNode);
        firstTime = false;
    }

    void update(float dt) {
        update(&rootNode, dt, glm::mat4(1.0f));
    }

    /*
    void setDefaultShaderProgram(GLuint shaderProgram) {
        setDefaultShaderProgram(&rootNode, shaderProgram);
    }
     */

private:

    void update(Spatial* node, float dt, glm::mat4 curTransform) {
        node->updateTransform();
        node->worldTransform = curTransform * node->localTransform;
        node->update(dt);
        for (Spatial* child : node->spatialChildren) {
            update(child, dt, node->worldTransform);
        }
    }

    void render(Node* node) {
        node->render();
        for (Node* child : node->children) {
            render(child);
        }
    }

    /*
    void setDefaultShaderProgram(Node* node, GLuint shaderProgram) {
        MeshNode* meshNode = dynamic_cast<MeshNode*>(node);
        if (meshNode != nullptr) {
            meshNode->setShaderProgram(shaderProgram);
        }
        for (Node* child : node->children) {
            setDefaultShaderProgram(child, shaderProgram);
        }
    }
     */

    Spatial rootNode;

    bool firstTime = true;
};




