#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    gl_Position = proj * view * model * vec4(iPos, 1.0);
    FragPos = vec3(model * vec4(iPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * iNormal;
    TexCoord = iTexCoord;
}