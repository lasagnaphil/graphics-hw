//
// Created by lasagnaphil on 2/8/18.
//

#ifndef GENGINE_FIRSTPERSONCAMERA_H
#define GENGINE_FIRSTPERSONCAMERA_H

#include <memory>
#include "Spatial.h"
#include "../Shader.h"
#include "../AppSettings.h"
#include "Camera.h"

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

class FirstPersonCamera : public Camera {
public:
    struct Settings {
        static constexpr float Yaw = -90.0f;
        static constexpr float Pitch = 0.0f;
        static constexpr float Speed = 10.0f;
        static constexpr float Sensitivity = 0.1f;
        static constexpr float Zoom = 45.0f;
    };

    FirstPersonCamera(float yaw = Settings::Yaw, float pitch = Settings::Pitch);

    virtual void update(float dt) override;
    virtual void processInput(SDL_Event& ev) override;

    float pitch;
    float yaw;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    IntRect viewport;
    bool constrainPitch;

    bool mouseMovementEnabled = true;

private:

    void updateCameraVectors();
};


#endif //GENGINE_CAMERA_H
