//
// Created by lasagnaphil on 4/7/18.
//

#ifndef GENGINE_TRACKBALLCAMERA_H
#define GENGINE_TRACKBALLCAMERA_H


#include "Spatial.h"
#include "Camera.h"

class TrackballCamera : public Camera {
public:
    TrackballCamera();

    virtual void update(float dt) override;
    virtual void render() override;
    virtual void processInput(SDL_Event& ev) override;

private:

    glm::vec3 calcMouseVec(glm::vec2 mousePos);

    Spatial* focusedNode;

    glm::mat4 perspectiveMatrix;
    float zoom = 45.0f;
    float radius = 300.0f;

    //debug
    float theta = 0.0f;

};


#endif //GENGINE_TRACKBALLCAMERA_H
