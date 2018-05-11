//
// Created by lasagnaphil on 5/1/18.
//

#include "Transform.h"

glm::mat4 Transform::toMat4() {
    glm::mat4 mat;
    mat = glm::scale(glm::mat4(1.0f), scale);
    mat = glm::mat4_cast(rotation) * mat;
    mat = glm::translate(glm::mat4(1.0f), position) * mat;
    return mat;
}
