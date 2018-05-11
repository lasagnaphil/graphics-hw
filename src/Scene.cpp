//
// Created by lasagnaphil on 2017-03-25.
//
#include "Scene.h"

void Scene::update(Spatial* node, float dt, glm::mat4 curTransform) {
    node->updateTransform();
    node->worldTransform = curTransform * node->localTransform;
    node->update(dt);
    for (Spatial* child : node->spatialChildren) {
        update(child, dt, node->worldTransform);
    }
}

void Scene::render(Node* node) {
    // render nodes recursively
    node->render();
    for (Node* child : node->children) {
        render(child);
    }

    // update remaining lights
    for (std::shared_ptr<Shader>& shader : shaders) {
        if (LightNode::numDirectionalLights== 0) {
            shader->setBool("dirLight.enabled", false);
        }
        for (unsigned int i = LightNode::numPointLights; i < LightNode::maxPointLights; ++i) {
            std::string pointLightStr = "pointLights[";
            pointLightStr.append(std::to_string(i));
            pointLightStr.append("]");

            shader->setBool((pointLightStr + ".enabled").c_str(), false);
        }
    }
}

void Scene::processInput(Node* node, SDL_Event& event) {
    node->processInput(event);
    for (Node* child : node->children) {
        processInput(child, event);
    }
}
