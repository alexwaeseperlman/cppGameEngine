#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 viewProjection;

out vec2 TexCoord;

void main()
{
    gl_Position = viewProjection * vec4(position.xy, -1.0, 1.0);
	TexCoord = texCoord;
}