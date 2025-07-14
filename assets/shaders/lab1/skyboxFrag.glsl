#version 460 core

out vec4 colour;

in vec3 texCoords;

uniform samplerCube u_skybox;

void main()
{
	colour = texture(u_skybox, texCoords);
}