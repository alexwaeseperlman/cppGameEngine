#version 330 core

layout(location = 0) out vec4 outColor;

in vec2 TexCoord;
uniform sampler2D spriteSheet;

void main()
{
    outColor = texture(spriteSheet, TexCoord);
}