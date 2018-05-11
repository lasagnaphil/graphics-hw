//
// Created by lasagnaphil on 2/13/18.
//

#ifndef GENGINE_LIGHTNODE_H
#define GENGINE_LIGHTNODE_H

#include "Spatial.h"
#include "../Shader.h"

class LightNode : public Spatial {
    friend class Scene;
public:
    enum class Type {
        Directional, Point, Spotlight
    };

    struct Attenuation {
        float constant;
        float linear;
        float quadratic;
    };

    LightNode(Type type = Type::Directional);

    virtual ~LightNode();

    void changeType(Type type);

    void update(float dt) override;


    // When type == Type::Point
    Attenuation attenuation;

    // When type == Type::Spotlight
    float cutOff;
    float outerCutOff;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;

    unsigned int getLightID() { return lightID; }

private:
    Type type;

    unsigned int lightID;

    static constexpr unsigned int maxDirectionalLights = 1;
    static constexpr unsigned int maxPointLights = 4;
    static constexpr unsigned int maxSpotLights = 4;
    static unsigned int numDirectionalLights;
    static unsigned int numPointLights;
    static unsigned int numSpotLights;
};



#endif //GENGINE_LIGHTNODE_H
