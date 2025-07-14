#version 460 core

out vec4 fragColour;

in float geo_distanceFromCam;

uniform vec3 u_particleColour;

void main()
{
	float darknessFactor = 1.0 - (geo_distanceFromCam / 100.0);

	vec3 finalColour = u_particleColour * darknessFactor;

	fragColour = vec4(finalColour, 1.0);
}