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

    glm::mat4 toMat4();
};


#endif //GENGINE_TRANSFORM_H
