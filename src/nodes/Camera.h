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

    virtual void processInput(SDL_Event& ev) override;

    inline glm::mat4 getViewMatrix() const {
        return glm::lookAt(getGlobalPosition(), getGlobalPosition() + getGlobalFrontVec(), getGlobalUpVec());
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

    bool mouseMovementEnabled = false;

private:

    void updateCameraVectors();
};


#endif //GENGINE_CAMERA_H
