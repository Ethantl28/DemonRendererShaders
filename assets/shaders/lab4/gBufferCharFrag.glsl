#version 460 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec2 gMetallicRoughness;

in vec3 vertexNormal;
in vec3 fragmentPos;
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D u_albedoMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_normalMap;

//global vars
vec3 normal;
vec3 albedoColour;
float specularStrength = 0.8;
vec3 viewDir;

void main()
{
	vec3 N = texture(u_normalMap, texCoord).rgb;
	normal = normalize(TBN * (N * 2.0 - 1.0));
	albedoColour = texture(u_albedoMap, texCoord).rgb;
	specularStrength = texture(u_specularMap, texCoord).r;


	gPosition = vec4(fragmentPos, 1.0);
	gNormal = vec4(N, 0.0);
	gDiffSpec = vec4(albedoColour, specularStrength);
	gMetallicRoughness = vec2(0.2, 0.85);
}