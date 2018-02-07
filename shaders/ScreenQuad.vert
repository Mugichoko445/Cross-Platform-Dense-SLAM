#version 440 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

out vec2 vsTexCoord;

uniform bool isYFlip;

void main()
{
	vsTexCoord = vec2(vTexCoord.x, isYFlip ? 1.0 - vTexCoord.y : vTexCoord.y);
	gl_Position = vec4(vPosition, 1.0);
}