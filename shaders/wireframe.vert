#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iBarycentric;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Barycentric;

void main()
{
    gl_Position = proj * view * model * vec4(iPos, 1.0);
    FragPos = vec3(model * vec4(iPos, 1.0));
    Barycentric = iBarycentric;
}
