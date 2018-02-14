//
// Created by lasagnaphil on 2/13/18.
//

#ifndef GENGINE_LIGHTNODE_H
#define GENGINE_LIGHTNODE_H


#include "Spatial.h"
#include "../Shader.h"

class LightNode : public Spatial {
public:
    LightNode();

    void update(float dt) override;

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

private:
};


#endif //GENGINE_LIGHTNODE_H
