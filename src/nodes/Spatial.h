//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_SPATIAL_H
#define GENGINE_SPATIAL_H

#include "Node.h"

class Spatial : public Node {
    friend class Scene;
public:
    Spatial() : Node(), position(position), spatialChildren(0) {
        spatialRepNode = this;
    }

    void setDirtyFlag(bool dirtyFlag);
    void updateTransform();

    void addSpatialChild(Spatial* spatial) {
        spatialChildren.push_back(spatial);
    }

    void setSpatialParent(Spatial* spatial) {
        spatialParent = spatial;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    void setPosition(const glm::vec3 &position) {
        this->position = position;
        setDirtyFlag(true);
    }

    void setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    glm::vec3 getScale() const {
        return scale;
    }

    void setScale(const glm::vec3 &scale) {
        this->scale = scale;
        setDirtyFlag(true);
    }

    void setScale(float x, float y, float z) {
        setScale(glm::vec3(x, y, z));
    }

    glm::vec3 getRotation() const {
        return rotation;
    }

    void setRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
        setDirtyFlag(true);
    }

    void setRotation(float x, float y, float z) {
        setScale(glm::vec3(x, y, z));
    }

    /*
    void setRotation(float angle, glm::vec3 axis) {
        setRotation(glm::rotate(glm::mat4(1.0f), angle, axis));
    }

    void setRotation(float angle, float axisX, float axisY, float axisZ) {
        setRotation(angle, glm::vec3(axisX, axisY, axisZ));
    }
     */

protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 localTransform;
    glm::mat4 worldTransform;

    bool dirtyFlag = true;

    std::vector<Spatial*> spatialChildren;
    Spatial* spatialParent;
};


#endif //GENGINE_SPATIAL_H
