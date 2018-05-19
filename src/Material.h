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
    enum class Type {
        SingleColor, Textured
    };

    Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular) {
        type = Type::SingleColor;
    }

    Material(std::vector<Texture> textures = {}, float shininess = 32.0f)
            : textures(std::move(textures)), shininess(shininess) {
        type = Type::Textured;
    }

    static std::shared_ptr<Material> create(std::initializer_list<Texture> textures = {}, float shininess = 32.0f) {
        return std::shared_ptr<Material>(new Material(textures, shininess));
    }

    static std::shared_ptr<Material> create(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) {
        return std::shared_ptr<Material>(new Material(ambient, diffuse, specular, shininess));
    }

    Type type;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;

    std::vector<Texture> textures;
};

#endif //GENGINE_MATERIAL_H
