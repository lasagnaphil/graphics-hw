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
        static constexpr float Yaw = -90.0f;
        static constexpr float Pitch = 0.0f;
        static constexpr float Speed = 2.5f;
        static constexpr float Sensitivity = 0.1f;
        static constexpr float Zoom = 45.0f;
    };

    Camera(float yaw = Settings::Yaw, float pitch = Settings::Pitch);

    virtual void update(float dt) override;

    void attachShader(Shader shader) {
        shaders.push_back(shader);
    }

    inline glm::vec3 getFrontVec() const {
        return glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    inline glm::vec3 getUpVec() const {
        return glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    inline glm::vec3 getRightVec() const {
        return glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    inline glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + getFrontVec(), getUpVec());
    }

    inline glm::mat4 getPerspectiveMatrix() const {
        return glm::perspective(
                glm::radians(zoom),
                (float)AppSettings::ScreenWidth / (float)AppSettings::ScreenHeight,
                0.1f,
                100.0f
        );
    }

    float pitch;
    float yaw;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    IntRect viewport;
    bool constrainPitch;

private:

    void updateCameraVectors();

    std::vector<Shader> shaders;
};


#endif //GENGINE_CAMERA_H
