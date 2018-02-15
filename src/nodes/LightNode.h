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
        Directional, Point
    };

    struct Attenuation {
        float constant;
        float linear;
        float quadratic;
    };

    LightNode(Type type = Type::Point);

    ~LightNode();

    void update(float dt) override;

    union {
        glm::vec3 direction;
        Attenuation attenuation;
    };

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    unsigned int getLightID() { return lightID; }

private:
    unsigned int lightID;
    Type type;

    static constexpr unsigned int maxDirectionalLights = 1;
    static constexpr unsigned int maxPointLights = 4;
    static unsigned int numDirectionalLights;
    static unsigned int numPointLights;
};



#endif //GENGINE_LIGHTNODE_H
