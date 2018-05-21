#version 330 core

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct DirLight {
    bool enabled;

    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float intensity;
};

struct PointLight {
    bool enabled;

    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float intensity;
};

struct SpotLight {
    bool enabled;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;

    float intensity;
};


out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 16
uniform PointLight pointLights[NR_POINT_LIGHTS];

#define NR_SPOT_LIGHTS 8
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform int numPointLights;
uniform int numSpotLights;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.intensity * light.diffuse * diff * material.diffuse;
    vec4 specular = light.intensity * light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.intensity * light.diffuse * diff * material.diffuse;
    vec4 specular = light.intensity * light.specular * spec * material.specular;

    return attenuation * (ambient + diffuse + specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.intensity * light.diffuse * diff * material.diffuse;
    vec4 specular = light.intensity * light.specular * spec * material.specular;

    return attenuation * (ambient + intensity * (diffuse + specular));
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result = vec4(0.0);
    if (dirLight.enabled) result = calcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < numPointLights; ++i) {
        if (pointLights[i].enabled) {
            result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    for (int i = 0; i < numSpotLights; ++i) {
        if (spotLights[i].enabled) {
            result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);
        }
    }

    FragColor = result;
}
