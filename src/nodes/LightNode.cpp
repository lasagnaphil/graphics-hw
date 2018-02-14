//
// Created by lasagnaphil on 2/13/18.
//

#include "LightNode.h"
#include "../Scene.h"

LightNode::LightNode()
        : Spatial(),
          ambientColor(1.0f, 1.0f, 1.0f),
          diffuseColor(1.0f, 1.0f, 1.0f),
          specularColor(1.0f, 1.0f, 1.0f)
{

}

void LightNode::update(float dt) {
    for (Shader shader : scene->getShaders()) {
        shader.use();
        shader.setVec3("light.position", position);
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", specularColor);
    }
}

