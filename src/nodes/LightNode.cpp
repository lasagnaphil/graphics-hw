//
// Created by lasagnaphil on 2/13/18.
//

#include "LightNode.h"
#include "../Scene.h"

unsigned int LightNode::numDirectionalLights = 0;
unsigned int LightNode::numPointLights = 0;
unsigned int LightNode::numSpotLights = 0;

LightNode::LightNode(Type type)
        : Spatial(),
          type(type),
          ambientColor(0.2f, 0.2f, 0.2f),
          diffuseColor(0.5f, 0.5f, 0.5f),
          specularColor(1.0f, 1.0f, 1.0f),
          attenuation({.constant = 1.0f, .linear = 0.09f, .quadratic = 0.032f}),
          cutOff(glm::cos(glm::radians(12.5f))),
          outerCutOff(glm::cos(glm::radians(17.5f)))
{
    switch(type) {
        case Type::Directional:
            if (numDirectionalLights >= maxDirectionalLights) {
                std::cerr << "Exceeded max number of directional lights!" << std::endl;
            }
            lightID = numDirectionalLights;
            numDirectionalLights++;
            break;
        case Type::Point:
            if (numPointLights >= maxPointLights) {
                std::cerr << "Exceeded max number of point lights!" << std::endl;
            }
            lightID = numPointLights;
            numPointLights++;
            break;
        case Type::Spotlight:
            if (numSpotLights >= maxSpotLights) {
                std::cerr << "Exceeded max number of spot lights!" << std::endl;
            }
            lightID = numSpotLights;
            numSpotLights++;
    }
}

void LightNode::changeType(Type type) {
    switch(this->type) {
        case Type::Directional:
            numDirectionalLights--; break;
        case Type::Point:
            numPointLights--; break;
        case Type::Spotlight:
            numSpotLights--; break;
    }
    switch(type) {
        case Type::Directional:
            if (numDirectionalLights >= maxDirectionalLights) {
                std::cerr << "Exceeded max number of directional lights!" << std::endl;
            }
            lightID = numDirectionalLights;
            numDirectionalLights++;
            break;
        case Type::Point:
            if (numPointLights >= maxPointLights) {
                std::cerr << "Exceeded max number of point lights!" << std::endl;
            }
            lightID = numPointLights;
            numPointLights++;
            break;
        case Type::Spotlight:
            if (numSpotLights >= maxSpotLights) {
                std::cerr << "Exceeded max number of spot lights!" << std::endl;
            }
            lightID = numSpotLights;
            numSpotLights++;
    }
    this->type = type;
}

void LightNode::update(float dt) {
    for (auto shader : scene->getShaders()) {
        shader->use();
        shader->setInt("numPointLights", numPointLights);
        shader->setInt("numSpotLights", numSpotLights);

        switch(type) {
            case Type::Directional: {
                shader->setVec3("dirLight.direction", getGlobalFrontVec());
                shader->setVec3("dirLight.ambient", ambientColor);
                shader->setVec3("dirLight.diffuse", diffuseColor);
                shader->setVec3("dirLight.specular", specularColor);
                break;
            }
            case Type::Point: {
                std::string pointLightStr = "pointLights[";
                pointLightStr.append(std::to_string(lightID));
                pointLightStr.append("]");
                shader->setVec3((pointLightStr + ".position").c_str(), getGlobalPosition());
                shader->setFloat((pointLightStr + ".constant").c_str(), attenuation.constant);
                shader->setFloat((pointLightStr + ".linear").c_str(), attenuation.linear);
                shader->setFloat((pointLightStr + ".quadratic").c_str(), attenuation.quadratic);
                shader->setVec3((pointLightStr + ".ambient").c_str(), ambientColor);
                shader->setVec3((pointLightStr + ".diffuse").c_str(), diffuseColor);
                shader->setVec3((pointLightStr + ".specular").c_str(), specularColor);
                break;
            }
            case Type::Spotlight: {
                auto transformMat = worldTransform;
                std::string spotLightStr = "spotLights[";
                spotLightStr.append(std::to_string(lightID));
                spotLightStr.append("]");

                shader->setVec3((spotLightStr + ".position").c_str(), getGlobalPosition());
                shader->setVec3((spotLightStr + ".direction").c_str(), getGlobalFrontVec());
                shader->setFloat((spotLightStr + ".cutOff").c_str(), cutOff);
                shader->setFloat((spotLightStr + ".outerCutOff").c_str(), outerCutOff);
                shader->setFloat((spotLightStr + ".constant").c_str(), attenuation.constant);
                shader->setFloat((spotLightStr + ".linear").c_str(), attenuation.linear);
                shader->setFloat((spotLightStr + ".quadratic").c_str(), attenuation.quadratic);
                shader->setVec3((spotLightStr + ".ambient").c_str(), ambientColor);
                shader->setVec3((spotLightStr + ".diffuse").c_str(), diffuseColor);
                shader->setVec3((spotLightStr + ".specular").c_str(), specularColor);
                break;
            }
        }
    }
}

LightNode::~LightNode() {
    if (type == Type::Directional) {
        numDirectionalLights--;
    }
    else if (type == Type::Point) {
        numPointLights--;
    }
}

