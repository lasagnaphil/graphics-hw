//
// Created by lasagnaphil on 2/14/18.
//

#ifndef GENGINE_MATERIAL_H
#define GENGINE_MATERIAL_H


#include <memory>
#include <glm/vec3.hpp>
#include <vector>
#include "Texture.h"

class Material {
public:
    Material(std::vector<Texture> textures = {}, float shininess = 32.0f)
            : shininess(shininess) {
        this->textures = std::move(textures);
    }

    static std::shared_ptr<Material> create(std::initializer_list<Texture> textures = {}, float shininess = 4.0f) {
        return std::shared_ptr<Material>(new Material(textures, shininess));
    }

    std::vector<Texture> textures;
    float shininess;
};

#endif //GENGINE_MATERIAL_H
