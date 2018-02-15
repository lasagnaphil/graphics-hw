//
// Created by lasagnaphil on 2/13/18.
//

#include "LightNode.h"
#include "../Scene.h"

unsigned int LightNode::numDirectionalLights = 0;
unsigned int LightNode::numPointLights = 0;

LightNode::LightNode(Type type)
        : Spatial(),
          type(type),
          ambientColor(1.0f, 1.0f, 1.0f),
          diffuseColor(1.0f, 1.0f, 1.0f),
          specularColor(1.0f, 1.0f, 1.0f)
{
    if (type == Type::Directional) {
        if (numDirectionalLights >= maxDirectionalLights) {
            std::cerr << "Exceeded max number of directional lights!" << std::endl;
        }
        lightID = numDirectionalLights;
        numDirectionalLights++;
    }
    else if (type == Type::Point) {
        if (numPointLights >= maxPointLights) {
            std::cerr << "Exceeded max number of point lights!" << std::endl;
        }
        lightID = numPointLights;
        numPointLights++;
    }
}

void LightNode::update(float dt) {
    for (Shader shader : scene->getShaders()) {
        shader.use();
        if (type == Type::Directional) {
            shader.setBool("dirLight.enabled", true);
            auto realDirection = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            auto inputDirection = glm::normalize(direction);
            shader.setVec3("dirLight.direction", realDirection);
            std::cout << realDirection.x << " " << realDirection.y << " " << realDirection.z << std::endl;
            std::cout << inputDirection.x << " " << inputDirection.y << " " << inputDirection.z << std::endl;
            std::cout << glm::degrees(glm::angle(glm::toQuat(glm::orientate3(realDirection)), glm::toQuat(glm::orientate3(inputDirection)))) << std::endl;
            //shader.setVec3("dirLight.directaon", direction);
            shader.setVec3("dirLight.ambient", ambientColor);
            shader.setVec3("dirLight.diffuse", diffuseColor);
            shader.setVec3("dirLight.specular", specularColor);
        }
        else if (type == Type::Point) {
            std::string pointLightStr = "pointLights[";
            pointLightStr.append(std::to_string(lightID));
            pointLightStr.append("]");

            shader.setBool((pointLightStr + ".enabled").c_str(), true);
            shader.setVec3((pointLightStr + ".position").c_str(), position);
            shader.setFloat((pointLightStr + ".constant").c_str(), attenuation.constant);
            shader.setFloat((pointLightStr + ".linear").c_str(), attenuation.linear);
            shader.setFloat((pointLightStr + ".quadratic").c_str(), attenuation.quadratic);
            shader.setVec3((pointLightStr + ".ambient").c_str(), ambientColor);
            shader.setVec3((pointLightStr + ".diffuse").c_str(), diffuseColor);
            shader.setVec3((pointLightStr + ".specular").c_str(), specularColor);
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

