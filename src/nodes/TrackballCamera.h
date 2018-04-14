//
// Created by lasagnaphil on 4/7/18.
//

#ifndef GENGINE_TRACKBALLCAMERA_H
#define GENGINE_TRACKBALLCAMERA_H


#include "Spatial.h"
#include "Camera.h"
#include "MeshNode.h"

class TrackballCamera : public Camera {
public:
    TrackballCamera();

    virtual void update(float dt) override;
    virtual void render() override;
    virtual void processInput(SDL_Event& ev) override;

private:

    glm::vec3 calcMouseVec(glm::vec2 mousePos);

    float radius = 300.0f;
    float distance = 10.0f;
    float translationSpeed = 10.0f;

    MeshNode::BoundingBox boundingBox;

    // debug
    float theta = 0.0f;

    // UI state
    bool enableZoom = false;
};


#endif //GENGINE_TRACKBALLCAMERA_H
