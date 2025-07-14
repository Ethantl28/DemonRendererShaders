#version 460 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec2 gMetallicRoughness;


in vec3 geo_fragmentPos;
in vec2 geo_texCoord;

uniform sampler2D u_perVertexNormals;

//global vars
vec3 normal;
vec3 albedoColour;
float specularStrength = 0.8;

//Height Colours
vec3 mountainColour = vec3(0.5, 0.5, 0.6);
vec3 grassColour = vec3(0.0, 0.5, 0.0);
float mountainHeight = 10.0;
float grassHeight = 0.0;

void main()
{
	normal = texture(u_perVertexNormals, geo_texCoord).rgb;

	float height = geo_fragmentPos.y;
	
	if (height >= mountainHeight)
	{
	    albedoColour = mountainColour;
	}
	else if (height >= grassHeight)
	{
	    float t = (height - grassHeight) / (mountainHeight - grassHeight);
	    albedoColour = mix(grassColour, mountainColour, t);
	}
	else
	{
	    albedoColour = grassColour;
	}



	gPosition = vec4(geo_fragmentPos, 1.0);
	gNormal = vec4(normal, 0.0);
	gDiffSpec = vec4(albedoColour, specularStrength);
	gMetallicRoughness = vec2(0.8, 0.5);
}