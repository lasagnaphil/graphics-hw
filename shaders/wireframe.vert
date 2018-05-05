#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iTexCoord;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;

void main()
{
    gl_Position = proj * view * model * vec4(iPos, 1.0);
    vs_out.normal = iNormal;
    FragPos = vec3(model * vec4(iPos, 1.0));
}
