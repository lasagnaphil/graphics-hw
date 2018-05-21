//
// Created by lasagnaphil on 2/13/18.
//

#include "LightNode.h"
#include "../Scene.h"

unsigned int LightNode::numDirectionalLights = 0;
unsigned int LightNode::numPointLights = 0;
unsigned int LightNode::numSpotLights = 0;

LightNode::LightNode(Type type, bool suppressWarnings)
        : Spatial(),
          type(type),
          ambientColor(0.2f, 0.2f, 0.2f, 1.0f),
          diffuseColor(0.5f, 0.5f, 0.5f, 1.0f),
          specularColor(1.0f, 1.0f, 1.0f, 1.0f),
          attenuation({.constant = 1.0f, .linear = 0.09f, .quadratic = 0.032f}),
          cutOff(glm::cos(glm::radians(12.5f))),
          outerCutOff(glm::cos(glm::radians(17.5f)))
{
    switch (type) {
        case Type::Directional:
            lightID = numDirectionalLights;
            numDirectionalLights++;
            break;
        case Type::Point:
            lightID = numPointLights;
            numPointLights++;
            break;
        case Type::Spotlight:
            lightID = numSpotLights;
            numSpotLights++;
    }
}

void LightNode::setEnabled(bool enabled) {
    this->isEnabled = enabled;
    for (auto shader : scene->getShaders()) {
        shader->use();
        switch(type) {
            case Type::Directional: {
                shader->setBool("dirLight.enabled", enabled);
                break;
            }
            case Type::Point: {
                std::string pointLightStr = "pointLights[";
                pointLightStr.append(std::to_string(lightID));
                pointLightStr.append("]");
                shader->setBool((pointLightStr + ".enabled").c_str(), enabled);
                break;
            }
            case Type::Spotlight: {
                std::string spotLightStr = "spotLights[";
                spotLightStr.append(std::to_string(lightID));
                spotLightStr.append("]");
                shader->setBool((spotLightStr + ".enabled").c_str(), enabled);
                break;
            }
        }
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
            lightID = numDirectionalLights;
            numDirectionalLights++;
            break;
        case Type::Point:
            lightID = numPointLights;
            numPointLights++;
            break;
        case Type::Spotlight:
            lightID = numSpotLights;
            numSpotLights++;
            break;
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
                shader->setVec4("dirLight.ambient", ambientColor);
                shader->setVec4("dirLight.diffuse", diffuseColor);
                shader->setVec4("dirLight.specular", specularColor);
                shader->setFloat("dirLight.intensity", intensity);
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
                shader->setVec4((pointLightStr + ".ambient").c_str(), ambientColor);
                shader->setVec4((pointLightStr + ".diffuse").c_str(), diffuseColor);
                shader->setVec4((pointLightStr + ".specular").c_str(), specularColor);
                shader->setFloat((pointLightStr + ".intensity").c_str(), intensity);
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
                shader->setVec4((spotLightStr + ".ambient").c_str(), ambientColor);
                shader->setVec4((spotLightStr + ".diffuse").c_str(), diffuseColor);
                shader->setVec4((spotLightStr + ".specular").c_str(), specularColor);
                shader->setFloat((spotLightStr + ".intensity").c_str(), intensity);
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
    else if (type == Type::Spotlight) {
        numSpotLights--;
    }
}


