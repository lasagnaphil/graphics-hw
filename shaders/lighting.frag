#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
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
};


out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

#define NR_SPOT_LIGHTS 4
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform int numPointLights;
uniform int numSpotLights;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec4 materialDiffuse = texture(material.texture_diffuse1, TexCoord);
    vec4 materialSpecular = texture(material.texture_specular1, TexCoord);

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = light.ambient * materialDiffuse;
    vec4 diffuse = light.diffuse * diff * materialDiffuse;
    vec4 specular = light.specular * spec * materialSpecular;

    return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec4 materialDiffuse = texture(material.texture_diffuse1, TexCoord);
    vec4 materialSpecular = texture(material.texture_specular1, TexCoord);

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = light.ambient * materialDiffuse;
    vec4 diffuse = light.diffuse * diff * materialDiffuse;
    vec4 specular = light.specular * spec * materialSpecular;

    return attenuation * (ambient + diffuse + specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec4 materialDiffuse = texture(material.texture_diffuse1, TexCoord);
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 materialSpecular = texture(material.texture_specular1, TexCoord);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = light.ambient * materialDiffuse;
    vec4 diffuse = light.diffuse * diff * materialDiffuse;
    vec4 specular = light.specular * spec * materialSpecular;

    return attenuation * (ambient + intensity * (diffuse + specular));
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result = calcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < numPointLights; ++i) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    for (int i = 0; i < numSpotLights; ++i) {
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }

    FragColor = result;
}
