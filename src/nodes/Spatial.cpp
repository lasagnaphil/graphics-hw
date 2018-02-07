//
// Created by lasagnaphil on 2/7/18.
//

#include "Spatial.h"

void Spatial::setDirtyFlag(bool dirtyFlag) {
    this->dirtyFlag = dirtyFlag;
    if (dirtyFlag) {
        for (Spatial* child : spatialChildren) {
            child->setDirtyFlag(dirtyFlag);
        }
    }
}

void Spatial::updateTransform() {
    if (dirtyFlag) {
        localTransform = glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), scale)
                * rotation;
        dirtyFlag = false;
    }
}
