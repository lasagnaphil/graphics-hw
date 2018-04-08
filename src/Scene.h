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

class Scene {
public:
    Scene() : rootNode(std::make_unique<Spatial>()) {
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

    std::vector<Shader> getShaders() const {
        return shaders;
    }

    void addShader(Shader shader) {
        shaders.push_back(shader);
    }

    template <typename T, typename ... Args>
    T* createNode(Args... args) {
        T* node = new T(args...);
        // node->scene = this;
        return node;
    }

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
        // render nodes recursively
        node->render();
        for (Node* child : node->children) {
            render(child);
        }

        // update remaining lights
        for (Shader& shader : shaders) {
            if (LightNode::numDirectionalLights== 0) {
                shader.setBool("dirLight.enabled", false);
            }
            for (unsigned int i = LightNode::numPointLights; i < LightNode::maxPointLights; ++i) {
                std::string pointLightStr = "pointLights[";
                pointLightStr.append(std::to_string(i));
                pointLightStr.append("]");

                shader.setBool((pointLightStr + ".enabled").c_str(), false);
            }
        }
    }

    void processInput(Node* node, SDL_Event& event) {
        node->processInput(event);
        for (Node* child : node->children) {
            processInput(child, event);
        }
    }

protected:
    std::unique_ptr<Spatial> rootNode;

    std::vector<Shader> shaders;

    bool firstTime = true;
};




