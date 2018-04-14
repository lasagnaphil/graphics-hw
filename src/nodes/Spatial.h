//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_SPATIAL_H
#define GENGINE_SPATIAL_H

#include "Node.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

class Spatial : public Node {
    friend class Scene;
public:
    Spatial() : Node(), position(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), spatialChildren(0) {
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

    void move(const glm::vec3& offset) {
        setPosition(position + offset);
    }

    void move(float x, float y, float z) {
        setPosition(position + glm::vec3(x, y, z));
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

    glm::quat getRotation() const {
        return rotation;
    }

    void setRotation(const glm::quat& rotation) {
        this->rotation = rotation;
        setDirtyFlag(true);
    }

    void setRotationEuler(const glm::vec3 rotation) {
        this->rotation = glm::quat(glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z));
        setDirtyFlag(true);
    }

    void rotate(float angleRadians, const glm::vec3& axis) {
        rotate(glm::angleAxis(angleRadians, axis));
    }

    void rotate(const glm::quat& rotation) {
        this->rotation = rotation * this->rotation;
        setDirtyFlag(true);
    }

    void rotateLocal(const glm::quat& rotation) {
        this->rotation = this->rotation * rotation;
        setDirtyFlag(true);
    }

    void pointAt(const glm::vec3& direction) {
        glm::vec3 dir = glm::normalize(direction);
        float scale = 1 / glm::length(glm::vec3(0.0f, 0.0f, 1.0f) + dir);
        float quatR = scale * (1 + dir.z);
        glm::vec3 quatI = scale * glm::cross({0.0f, 0.0f, 1.0f}, dir);
        this->rotation = glm::quat(quatR, quatI.x, quatI.y, quatI.z);
        setDirtyFlag(true);
    }

    void pointAt(float x, float y, float z) {
        pointAt({x, y, z});
    }

    glm::vec3 getGlobalPosition() const {
        return glm::vec3(worldTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        /*
        using namespace glm;

        // Get the 3 basis vector planes at the camera origin and transform them into model space.
        //
        // NOTE: Planes have to be transformed by the inverse transpose of a matrix
        //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
        //
        //       So for a transform to model space we need to do:
        //            inverse(transpose(inverse(MV)))
        //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
        //
        // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
        //
        mat4 modelViewT = transpose(worldTransform);

        // Get plane normals
        vec3 n1(modelViewT[0]);
        vec3 n2(modelViewT[1]);
        vec3 n3(modelViewT[2]);

        // Get plane distances
        float d1(modelViewT[0].w);
        float d2(modelViewT[1].w);
        float d3(modelViewT[2].w);

        // Get the intersection of these 3 planes
        // http://paulbourke.net/geometry/3planes/
        vec3 n2n3 = cross(n2, n3);
        vec3 n3n1 = cross(n3, n1);
        vec3 n1n2 = cross(n1, n2);

        vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
        float denom = dot(n1, n2n3);

        return top / denom;
         */
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

    glm::vec3 getGlobalRotation() const {

    }
    glm::vec3 getGlobalScale() const {
        if (spatialParent) {
            return spatialParent->getGlobalScale() * scale;
        }
        else {
            return scale;
        }
    }

    void setGlobalScale(const glm::vec3& newScale) {
        auto globalScale = getGlobalScale();
        scale.x *= newScale.x / globalScale.x;
        scale.y *= newScale.y / globalScale.y;
        scale.z *= newScale.z / globalScale.z;
        setDirtyFlag(true);
    }

    void setGlobalScale(float x, float y, float z) {
        setGlobalScale({x, y, z});
    }

    std::vector<Spatial*> spatialChildren;
    Spatial* spatialParent;

protected:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 localTransform;
    glm::mat4 worldTransform;

    bool dirtyFlag = true;
};


#endif //GENGINE_SPATIAL_H
