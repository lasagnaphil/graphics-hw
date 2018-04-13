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

TrackballCamera::TrackballCamera() {
}

void TrackballCamera::update(float dt) {
    static glm::vec2 prevMousePos;
    static bool started = false;

    auto inputMgr = InputManager::getInstance();

    if (inputMgr->mousePressed(SDL_BUTTON_LEFT)) {
        auto displaySize = ImGui::GetIO().DisplaySize;
        glm::vec2 mousePos = inputMgr->mousePos();
        mousePos.x -= displaySize.x / 2;
        mousePos.y -= displaySize.y / 2;

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

        spatialParent->rotateLocal(q);

        prevMousePos = mousePos;
    }
    else {
        started = false;
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
    ImGui::Checkbox("Use zoom instead of dolly", &enableZoom);
    ImGui::End();
}

void TrackballCamera::processInput(SDL_Event& ev) {
    if (ev.type == SDL_MOUSEWHEEL) {
        if (enableZoom) {
            zoom += ev.wheel.y;
        }
        else {
            float increment = 0.1f * ev.wheel.y;
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

