//
// Created by lasagnaphil on 4/7/18.
//

#include <imgui.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>

#include "TrackballCamera.h"

#include "../Scene.h"
#include "../InputManager.h"
#include "../AppSettings.h"
#include "MeshNode.h"

TrackballCamera::TrackballCamera() {
}

void TrackballCamera::update(float dt) {
    static glm::vec2 prevMousePos;
    static bool started = false;

    auto inputMgr = InputManager::getInstance();

    auto trackballFocus = this->spatialParent->spatialParent;
    auto trackballPivot = this->spatialParent;

    if (inputMgr->mousePressed(SDL_BUTTON_LEFT)) {

        auto displaySize = ImGui::GetIO().DisplaySize;
        glm::vec2 mousePos = inputMgr->mousePos();
        mousePos.x -= displaySize.x / 2;
        mousePos.y -= displaySize.y / 2;
        mousePos.y *= -1;

        if (!started) {
            started = true;
            prevMousePos = mousePos;
        }

        glm::vec3 v1 = calcMouseVec(prevMousePos);
        glm::vec3 v2 = calcMouseVec(mousePos);

        glm::quat q;
        float dotProduct = glm::dot(v2, v1);
        float lengthProduct = std::sqrt(glm::length2(v1) * glm::length2(v2));
        if (dotProduct / lengthProduct == -1) {
            q = glm::quat(1.f, 0.f, 0.f, 0.f);
        }
        else {
            q = glm::quat(lengthProduct + dotProduct, glm::cross(v2, v1));
            q = glm::normalize(q);
        }

        trackballPivot->rotateLocal(q);

        prevMousePos = mousePos;
    }
    else {
        started = false;
    }

    auto upDir = getGlobalUpVec();
    auto rightDir = getGlobalRightVec();
    if (inputMgr->keyboardPressed(SDLK_LEFT)) {
        trackballFocus->move(-translationSpeed * dt * rightDir);
    }
    else if (inputMgr->keyboardPressed(SDLK_RIGHT)) {
        trackballFocus->move(translationSpeed * dt * rightDir);
    }
    else if (inputMgr->keyboardPressed(SDLK_UP)) {
        trackballFocus->move(translationSpeed * dt * upDir);
    }
    else if (inputMgr->keyboardPressed(SDLK_DOWN)) {
        trackballFocus->move(-translationSpeed * dt * upDir);
    }

    Camera::update(dt);
}

void TrackballCamera::render() {
    auto inputMgr = InputManager::getInstance();
    ImGui::Begin("Trackball");
    ImGui::Text("Position of trackball focus: %s", glm::to_string(spatialParent->getGlobalPosition()).c_str());
    ImGui::Text("Position of trackball camera: %s", glm::to_string(getGlobalPosition()).c_str());
    ImGui::Text("Theta: %f", glm::degrees(theta));
    ImGui::Text("GlobalFrontVec: %s", glm::to_string(getGlobalFrontVec()).c_str());
    ImGui::Text("GlobalUpVec: %s", glm::to_string(getGlobalUpVec()).c_str());
    ImGui::Text("Distance: %f", distance);
    ImGui::Text("Zoom: %f", zoom);
    if (ImGui::Button("Fit To Screen")) {
        auto mobile = scene->getRootNode()->query("mobile")->cast<MeshNode>();
        glm::mat4 projMatrix = getPerspectiveMatrix();
        glm::mat4 viewMatrix = getViewMatrix();
        glm::mat4 projViewMatrix = projMatrix * viewMatrix;
        boundingBox = mobile->findBoundingBox(projViewMatrix);

        auto trackballFocus = this->spatialParent->spatialParent;
        auto trackballPivot = this->spatialParent;

        glm::vec4 x1 = glm::inverse(projViewMatrix) * glm::vec4(boundingBox.leftTop, 1.0f, 1.0f);
        glm::vec4 x2 = glm::inverse(projViewMatrix) * glm::vec4(boundingBox.rightBottom, 1.0f, 1.0f);
        glm::vec4 y1 = glm::inverse(projMatrix) * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
        glm::vec4 y2 = glm::inverse(projMatrix) * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

        float r1 = (y1 - glm::mat4(trackballFocus->getRotation()) * x1).z;
        float r2 = (y2 - glm::mat4(trackballFocus->getRotation()) * x2).z;
        printf("r1 = %f, r2 = %f\n", r1, r2);
    }
    ImGui::Text("Bounding Box: Left %f Right %f Bottom %f Top %f",
                boundingBox.leftTop.x, boundingBox.rightBottom.x, boundingBox.leftTop.y, boundingBox.rightBottom.y);
    ImGui::Checkbox("Use zoom instead of dolly", &enableZoom);
    ImGui::End();
}

void TrackballCamera::processInput(SDL_Event& ev) {
    if (ev.type == SDL_MOUSEWHEEL) {
        if (enableZoom) {
            zoom += ev.wheel.y;
        }
        else {
            float increment = 1.0f * ev.wheel.y;
            if (distance + increment > 0.f) {
                distance += increment;
            }
            auto curPos = getPosition();
            auto nextPos = distance / glm::length(curPos) * curPos;
            setPosition(nextPos);
        }
    }
}

glm::vec3 TrackballCamera::calcMouseVec(glm::vec2 mousePos) {
    float z;
    float mouseRadiusSq = mousePos.x * mousePos.x + mousePos.y + mousePos.y;
    float radiusSq = radius * radius;
    if (mouseRadiusSq <= 0.5f * radiusSq) {
        z = std::sqrt(radiusSq - mouseRadiusSq);
    }
    else {
        z = 0.5f * radiusSq / std::sqrt(mouseRadiusSq);
    }

    return glm::normalize(glm::vec3(mousePos.x, mousePos.y, z));
}

