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
#include "Shader.h"
#include "nodes/LightNode.h"
#include "BSPTree.h"
#include "nodes/Camera.h"

class Scene {
public:
    Scene() : rootNode(std::make_unique<Spatial>()) {
        rootNode->scene = this;
    }

    Node* getRootNode() { return rootNode.get(); }

    void render();

    void update(float dt);

    void updateTransforms();

    void processInput(SDL_Event& event) {
        processInput(rootNode.get(), event);
    }

    std::vector<std::shared_ptr<Shader>> getShaders() const {
        return shaders;
    }

    void addShader(std::shared_ptr<Shader> shader) {
        shaders.push_back(shader);
    }

    void constructBSPTree();

    void setMainCamera(Camera* camera) {
        this->mainCamera = camera;
    }

private:
    void update(Spatial* node, float dt, glm::mat4 curTransform);
    void updateTransforms(Spatial* node, glm::mat4 curTransform);
    void render(Node* node);
    void processInput(Node* node, SDL_Event& event);

protected:
    std::unique_ptr<Spatial> rootNode;

    std::vector<std::shared_ptr<Shader>> shaders;

    Camera* mainCamera;

    BSPTree bspTree;

    bool firstTime = true;
};




