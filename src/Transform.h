//
// Created by lasagnaphil on 5/1/18.
//

#ifndef GENGINE_TRANSFORM_H
#define GENGINE_TRANSFORM_H


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 toMat4() {
        glm::mat4 mat;
        mat = glm::scale(glm::mat4(1.0f), scale);
        mat = glm::mat4_cast(rotation) * mat;
        mat = glm::translate(mat, position);
        return mat;
    }
};


#endif //GENGINE_TRANSFORM_H
