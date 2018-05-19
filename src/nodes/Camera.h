//
// Created by lasagnaphil on 4/7/18.
//

#ifndef GENGINE_CAMERA_H
#define GENGINE_CAMERA_H


#include "Spatial.h"
#include "../AppSettings.h"

class Camera : public Spatial {
public:
    inline glm::mat4 getViewMatrix() const;

    inline glm::mat4 getPerspectiveMatrix() const;

    virtual void update(float dt) override;

protected:
    float zoom = 45.0f;
};


#endif //GENGINE_CAMERA_H
