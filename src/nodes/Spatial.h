//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_SPATIAL_H
#define GENGINE_SPATIAL_H

#include "Node.h"
#include "../Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

class Spatial : public Node {
    friend class Scene;
public:
    Spatial() : Node(),
                spatialChildren(0) {
        spatialRepNode = this;
        transform = {
                .position = {0.0f, 0.0f, 0.0f},
                .rotation = {1.0f, 0.0f, 0.0f, 0.0f},
                .scale = {1.0f, 1.0f, 1.0f}
        };
    }

    void setDirtyFlag(bool dirtyFlag);
    void updateTransform();

    void addSpatialChild(Spatial* spatial) {
        spatialChildren.push_back(spatial);
    }

    void setSpatialParent(Spatial* spatial) {
        spatialParent = spatial;
    }

    Transform getTransform() const {
        return transform;
    }

    void setTransform(const Transform& transform) {
        this->transform = transform;
    }

    glm::vec3 getPosition() const {
        return transform.position;
    }

    void setPosition(const glm::vec3 &position) {
        this->transform.position = position;
        setDirtyFlag(true);
    }

    void setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    void move(const glm::vec3& offset) {
        setPosition(transform.position + offset);
    }

    void move(float x, float y, float z) {
        setPosition(transform.position + glm::vec3(x, y, z));
    }

    glm::vec3 getScale() const {
        return transform.scale;
    }

    void setScale(const glm::vec3 &scale) {
        transform.scale = scale;
        setDirtyFlag(true);
    }

    void setScale(float x, float y, float z) {
        setScale(glm::vec3(x, y, z));
    }

    glm::quat getRotation() const {
        return transform.rotation;
    }

    void setRotation(const glm::quat& rotation) {
        transform.rotation = rotation;
        setDirtyFlag(true);
    }

    void setRotationEuler(const glm::vec3 rotation) {
        transform.rotation = glm::quat(glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z));
        setDirtyFlag(true);
    }

    void rotate(float angleRadians, const glm::vec3& axis) {
        rotate(glm::angleAxis(angleRadians, axis));
    }

    void rotate(const glm::quat& rotation) {
        transform.rotation = rotation * transform.rotation;
        setDirtyFlag(true);
    }

    void rotateLocal(const glm::quat& rotation) {
        transform.rotation = transform.rotation * rotation;
        setDirtyFlag(true);
    }

    void pointAt(const glm::vec3& direction) {
        glm::vec3 dir = glm::normalize(direction);
        float scale = 1 / glm::length(glm::vec3(0.0f, 0.0f, 1.0f) + dir);
        float quatR = scale * (1 + dir.z);
        glm::vec3 quatI = scale * glm::cross({0.0f, 0.0f, 1.0f}, dir);
        transform.rotation = glm::quat(quatR, quatI.x, quatI.y, quatI.z);
        setDirtyFlag(true);
    }

    void pointAt(float x, float y, float z) {
        pointAt({x, y, z});
    }

    glm::vec3 getGlobalPosition() const {
        return glm::vec3(worldTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    glm::vec3 getGlobalFrontVec() const {
        return glm::normalize(glm::vec3(worldTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
    }

    glm::vec3 getGlobalUpVec() const {
        return glm::normalize(glm::vec3(worldTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
    }

    glm::vec3 getGlobalRightVec() const {
        return glm::normalize(glm::vec3(worldTransform * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
    }

    glm::vec3 getFrontVec() const {
        return glm::normalize(glm::vec3(localTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
        // return glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::vec3 getUpVec() const {
        return glm::normalize(glm::vec3(localTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        // return glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 getRightVec() const {
        return glm::normalize(glm::vec3(localTransform * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        // return glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::mat4 getLocalTransform() const {
        return localTransform;
    }

    glm::mat4 getWorldTransform() const {
        return worldTransform;
    }

    glm::vec3 getGlobalScale() const {
        if (spatialParent) {
            return spatialParent->getGlobalScale() * transform.scale;
        }
        else {
            return transform.scale;
        }
    }

    void setGlobalScale(const glm::vec3& newScale) {
        auto globalScale = getGlobalScale();
        transform.scale.x *= newScale.x / globalScale.x;
        transform.scale.y *= newScale.y / globalScale.y;
        transform.scale.z *= newScale.z / globalScale.z;
        setDirtyFlag(true);
    }

    void setGlobalScale(float x, float y, float z) {
        setGlobalScale({x, y, z});
    }

    std::vector<Spatial*> spatialChildren;
    Spatial* spatialParent;

protected:
    Transform transform;

    glm::mat4 localTransform;
    glm::mat4 worldTransform;

    bool dirtyFlag = true;
};


#endif //GENGINE_SPATIAL_H
