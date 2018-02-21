//
// Created by lasagnaphil on 2/14/18.
//

#ifndef GENGINE_MATERIAL_H
#define GENGINE_MATERIAL_H


#include <memory>
#include <variant>
#include <glm/vec3.hpp>
#include <vector>
#include "Texture.h"

class Material {
public:
    Material(std::initializer_list<Texture> texturesToInsert = {}, float shininess = 32.0f)
            : shininess(shininess)
    {
        textures.insert(textures.end(), texturesToInsert.begin(), texturesToInsert.end());
    }

    std::vector<Texture> textures;
    float shininess;
};

#endif //GENGINE_MATERIAL_H
