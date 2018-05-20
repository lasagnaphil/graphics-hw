//
// Created by lasagnaphil on 2017-03-25.
//
#include <chrono>
#include "Scene.h"

void Scene::update(float dt) {
    update(rootNode.get(), dt, glm::mat4(1.0f));
}

void Scene::update(Spatial* node, float dt, glm::mat4 curTransform) {
    if (!node->getEnabled()) return;
    node->updateTransform();
    node->worldTransform = curTransform * node->localTransform;
    node->update(dt);
    for (Spatial* child : node->spatialChildren) {
        update(child, dt, node->worldTransform);
    }
}

void Scene::updateTransforms() {
    updateTransforms(rootNode.get(), glm::mat4(1.0f));
}

void Scene::updateTransforms(Spatial* node, glm::mat4 curTransform) {
    node->updateTransform();
    node->worldTransform = curTransform * node->localTransform;
    for (Spatial* child : node->spatialChildren) {
        updateTransforms(child, node->worldTransform);
    }
}

void Scene::render() {
    using std::string;

    // update remaining lights
    for (std::shared_ptr<Shader>& shader : shaders) {
        if (LightNode::numDirectionalLights == 0) {
            shader->setBool("dirLight.enabled", false);
        }
        for (unsigned int i = LightNode::numPointLights; i < LightNode::maxPointLights; ++i) {
            std::string pointLightStr = "pointLights[";
            pointLightStr.append(std::to_string(i));
            pointLightStr.append("]");

            shader->setBool((pointLightStr + ".enabled").c_str(), false);
        }
        for (unsigned int i = LightNode::numSpotLights; i < LightNode::maxSpotLights; ++i) {
            std::string spotLightStr = "spotLights[";
            spotLightStr.append(std::to_string(i));
            spotLightStr.append("]");

            shader->setBool((spotLightStr+ ".enabled").c_str(), false);
        }
    }

    bspTree.sortVertices(mainCamera->getGlobalPosition());

    render(rootNode.get());
    firstTime = false;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Now draw triangles from BSP Tree
    bspTree.render();
    glDisable(GL_BLEND);
}

void Scene::render(Node* node) {
    if (!node->getEnabled()) return;
    // render nodes recursively
    node->render();
    for (Node* child : node->children) {
        render(child);
    }

}

void Scene::processInput(Node* node, SDL_Event& event) {
    if (!node->getEnabled()) return;
    node->processInput(event);
    for (Node* child : node->children) {
        processInput(child, event);
    }
}

void Scene::constructBSPTree() {
    updateTransforms();
    bspTree.build(rootNode.get());
}
