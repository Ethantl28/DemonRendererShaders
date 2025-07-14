#version 460 core

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;

out vec3 vs_fragmentPos;
out vec2 vs_texCoord;

uniform mat4 u_model;

void main()
{
	vs_fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	vs_texCoord = a_texCoord;
}