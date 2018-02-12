//
// Created by lasagnaphil on 2/13/18.
//

#ifndef GENGINE_LIGHTNODE_H
#define GENGINE_LIGHTNODE_H


#include "Spatial.h"
#include "../Shader.h"

class LightNode : public Spatial {
public:
    LightNode() : Spatial() {}

    void update(float dt) override;

    glm::vec3 color;

private:
    std::vector<Shader> shaders;
};


#endif //GENGINE_LIGHTNODE_H
