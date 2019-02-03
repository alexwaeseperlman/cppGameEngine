#version 410 core

layout(location = 0) in vec2 position;
uniform int spriteNumber;
uniform vec2 gridSize;
uniform sampler2D spriteSheet;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}