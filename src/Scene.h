#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include <stack>
#include <functional>
#include <iostream>
#include <memory>
#include "nodes/Node.h"
#include "nodes/Spatial.h"
#include "nodes/Camera.h"

class Scene {
public:
    Scene() : rootNode(std::make_unique<Spatial>()), cameras(0) {
        rootNode->scene = this;
    }

    Node* getRootNode() { return rootNode.get(); }

    void render() {
        render(rootNode.get());
        firstTime = false;
    }

    void update(float dt) {
        update(rootNode.get(), dt, glm::mat4(1.0f));
    }

    void processInput(SDL_Event& event) {
        processInput(rootNode.get(), event);
    }

    template <typename T, typename ... Args>
    T* createNode(Args... args) {
        T* node = new T(args...);
        node->scene = this;
        Camera* camera = dynamic_cast<Camera*>(node);
        if (camera) { cameras.push_back(camera); }
        return node;
    }

    template <typename ... Args>
    Camera* createCamera(Args... args) {
        Camera* camera = new Camera(args...);
        camera->scene = this;
        cameras.push_back(camera);
        return camera;
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
        // Update shaders based on camera
        for (auto camera : cameras) {
            camera->cameraUpdate();
        }

        // render nodes recursively
        node->render();
        for (Node* child : node->children) {
            render(child);
        }
    }

    void processInput(Node* node, SDL_Event& event) {
        node->processInput(event);
        for (Node* child : node->children) {
            processInput(child, event);
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

protected:
    std::unique_ptr<Spatial> rootNode;

    std::vector<Camera*> cameras;

    bool firstTime = true;
};




