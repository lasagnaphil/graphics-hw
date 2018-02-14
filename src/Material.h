//
// Created by lasagnaphil on 2/14/18.
//

#ifndef GENGINE_MATERIAL_H
#define GENGINE_MATERIAL_H


#include <memory>
#include <variant>
#include <glm/vec3.hpp>
#include "Texture.h"

class Material {
public:
    using Vec3OrTexture = std::variant<glm::vec3, Texture>;

    Material(Texture diffuseMap, Texture specularMap, float shininess = 32.0f)
            : shininess(shininess) {
        diffuse = std::move(diffuseMap);
        specular = std::move(specularMap);
    }

    Texture diffuse;
    Texture specular;

    float shininess;
};

#endif //GENGINE_MATERIAL_H
