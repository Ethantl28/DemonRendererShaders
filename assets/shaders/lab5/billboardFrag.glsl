#version 460 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec2 gMetallicRoughness;

in vec2 texCoord;
in vec3 vertexNormal;
in vec3 fragmentPos;

uniform sampler2D u_billboardImage;

vec3 normal;
vec3 albedoColour;
float specStrength = 0.8;

void main()
{
	vec4 imageColour = texture(u_billboardImage, texCoord);
	float alphaMask = step(0.0001, imageColour.a);
	if (alphaMask == 0.0) discard;

	normal = normalize(vertexNormal);

	gPosition = vec4(fragmentPos, 1.0);
	gNormal = vec4(normal, 0.0);
	gDiffSpec = vec4(imageColour.rgb, specStrength);
	gMetallicRoughness = vec2(0.0, 0.65);
}