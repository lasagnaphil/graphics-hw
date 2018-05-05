//
// Created by lasagnaphil on 4/7/18.
//

#ifndef GENGINE_CAMERA_H
#define GENGINE_CAMERA_H


#include "Spatial.h"
#include "../AppSettings.h"

class Camera : public Spatial {
public:
    inline glm::mat4 getViewMatrix() const {
        return glm::lookAt(getGlobalPosition(), getGlobalPosition() - getGlobalFrontVec(), getGlobalUpVec());
    }

    inline glm::mat4 getPerspectiveMatrix() const {
        return glm::perspective(
                glm::radians(zoom),
                (float)AppSettings::ScreenWidth / (float)AppSettings::ScreenHeight,
                0.1f,
                1000.0f
        );
    }

    virtual void update(float dt) override;

protected:
    float zoom = 45.0f;
};


#endif //GENGINE_CAMERA_H
