//
// Created by lasagnaphil on 2/8/18.
//

#ifndef GENGINE_CAMERA_H
#define GENGINE_CAMERA_H

#include <memory>
#include "Spatial.h"
#include "../Shader.h"
#include "../AppSettings.h"

enum CameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

struct IntRect {
    int x;
    int y;
    int width;
    int height;
};

class Camera : public Spatial {
public:
    struct Settings {
        static constexpr float Speed = 2.5f;
        static constexpr float Sensitivity = 0.001f;
        static constexpr float Zoom = 45.0f;
    };
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    IntRect viewport;

    Camera();

    void cameraUpdate();

    virtual void update(float dt) override;

    virtual void processInput(SDL_Event& event) override;

    void attachShader(Shader shader) {
        shaders.push_back(shader);
    }

    glm::vec3 getFrontVec() {
        return glm::orientate4(rotation) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    glm::vec3 getUpVec() {
        return glm::orientate4(rotation) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    glm::vec3 getRightVec() {
        return glm::orientate4(rotation) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + getFrontVec(), getUpVec());
    }

    glm::mat4 getPerspectiveMatrix() {
        return glm::perspective(
                glm::radians(zoom),
                (float)AppSettings::ScreenWidth / (float)AppSettings::ScreenHeight,
                0.1f,
                100.0f
        );
    }

private:

    std::vector<Shader> shaders;
};


#endif //GENGINE_CAMERA_H
