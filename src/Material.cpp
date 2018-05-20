//
// Created by lasagnaphil on 2/14/18.
//

#include "Material.h"

Material::Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
    type = Type::SingleColor;
}

Material::Material(std::vector<Texture> textures, float shininess)
        : textures(std::move(textures)), shininess(shininess) {
    type = Type::Textured;
}

std::shared_ptr<Material> Material::create(std::initializer_list<Texture> textures, float shininess) {
    return std::shared_ptr<Material>(new Material(textures, shininess));
}

std::shared_ptr<Material> Material::create(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess) {
    return std::shared_ptr<Material>(new Material(ambient, diffuse, specular, shininess));
}
