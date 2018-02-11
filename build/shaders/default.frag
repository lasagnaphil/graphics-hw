#version 330 core

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
    outColor = texture(ourTexture, texCoord);
}
