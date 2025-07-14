#version 460 core

layout(location = 0) out vec4 outputColour;


in vec2 texCoord;

const float PI = 3.1415;


struct directionalLight
{
	vec3 colour;
	vec3 direction;
};

struct pointLight
{
	vec3 colour;
	vec3 position;
	vec3 constants;
};

struct spotLight
{
	vec3 colour;
	vec3 position;
	vec3 direction;
	vec3 constants;
	float cutOff;
	float outerCutOff;
};

const int numPointLights = 10;
const int numSpotLights = 1;

//layout(std140, binding = 69) uniform b_camera
//{
//	uniform mat4 u_view;
//	uniform mat4 u_projection;
//	uniform vec3 u_viewPos;
//};

layout(std140, binding = 2) uniform b_lights
{
	uniform directionalLight dLight;
	uniform pointLight pLights[numPointLights];
	uniform spotLight sLights[numSpotLights];
};


uniform sampler2D u_shadowMap;
uniform sampler2D u_posInWs;
uniform sampler2D u_normal;
uniform sampler2D u_diffSpec;
uniform mat4 u_lightSpaceTransform;
uniform sampler2D u_depth;
uniform sampler2D u_skyboxColour;
uniform vec3 u_viewPos;
uniform sampler2D u_metallicRoughness;

uniform float u_pLightsAmbient = 0;
uniform float u_pLightsShine = 64;

vec3 getDirectionalLight();
vec3 getPointLight(int idx);
vec3 getSpotLight(int idx);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(float NdotH);
float GeometrySmith(float NdotV, float NdotL);
float GeometrySchlickGGX(float Ndot);


//global vars
vec3 normal;
vec3 albedoColour;
float specularStrength = 0.8;
vec3 viewDir;
vec4 fragPosLightSpace;
vec3 posInWs;
float depth;
vec3 skyboxColour;
float metallic;
float roughness;
vec3 F0;

void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	//Get data from gbuffer
	posInWs = texture(u_posInWs, texCoord).rgb;
	normal = texture(u_normal, texCoord).rgb;
	albedoColour = texture(u_diffSpec, texCoord).rgb;
	specularStrength = texture(u_diffSpec, texCoord).a;
	metallic = texture(u_metallicRoughness, texCoord).r;
	roughness = texture(u_metallicRoughness, texCoord).g;

	//Light space calculation
	fragPosLightSpace = u_lightSpaceTransform * vec4(posInWs, 1.0);

	//View direction
	viewDir = normalize(u_viewPos - posInWs);

	// light casters
	result += getDirectionalLight() / 3.0;

	for (int i = 0; i < numPointLights; i++)
	{
		result += getPointLight(i);
	}

	for (int i = 0; i < numSpotLights; i++)
	{
		//result += getSpotLight(i);
	}

	//Setup depth
	depth = texture(u_depth, texCoord).z;

	//Setup skybox colour
	skyboxColour = texture(u_skyboxColour, texCoord).rgb;

	//If the pixel is far away, then set the output to the skybox colour
	if (depth >= 0.9999f)
	{
		outputColour = vec4(skyboxColour, 1.0);
	}
	else
	{
		outputColour = vec4(result * albedoColour, 1.0);
	}

	//outputColour = vec4(vec3(1.0), 1.0);
	//outputColour = vec4(albedoColour, 1.0);
}

float CalculateShadowAmount()
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	vec2 texelSize = vec2(1.0 / 4096.0);

	projCoords = projCoords * 0.5 + 0.5;

	float total;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float closestDepth = texture(u_shadowMap, projCoords.xy + (vec2(x, y) * texelSize)).r;

			float currentDepth = projCoords.z;

			if (currentDepth > closestDepth + 0.004)
			{
				total += 1;
			}
		}
	}

	return total / 9;
}

vec3 getDirectionalLight()
{
	F0 = mix(vec3(0.04), albedoColour, metallic);

	vec3 L = normalize(-dLight.direction);
	vec3 H = normalize(L + viewDir);

	float NdotL = max(dot(normal, L), 0.0001);
	float NdotH = max(dot(normal, H), 0.0001);
	float NdotV = max(dot(normal, viewDir), 0.0001);

	vec3 F = fresnelSchlick(NdotH, F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse = (kD * albedoColour / PI) * NdotL;
	
	//Specular
	float D = DistributionGGX(NdotH);
	float G = GeometrySmith(NdotV, NdotL);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular = numerator / denominator;

	//Ambient
	vec3 ambient = 0.09 * vec3(1.0) * albedoColour;

	return ambient + (1.0 - CalculateShadowAmount()) + (specular + diffuse) * (dLight.colour);
}

vec3 getPointLight(int idx)
{
	float distance = length(pLights[idx].position - posInWs) / pLights[idx].constants.x;
	float attn = (1.0 / (distance * distance));

	F0 = mix(vec3(0.04), albedoColour, metallic);

	vec3 L = normalize((pLights[idx].position) - posInWs);
	vec3 H = normalize(L + viewDir);

	float NdotL = max(dot(normal, L), 0.0001);
	float NdotH = max(dot(normal, H), 0.0001);
	float NdotV = max(dot(normal, viewDir), 0.0001);

	vec3 F = fresnelSchlick(NdotH, F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse = (kD * albedoColour / PI) * NdotL;

	//Specular
	float D = DistributionGGX(NdotH);
	float G = GeometrySmith(NdotV, NdotL);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular = numerator / denominator;

	//Ambient
	vec3 ambient = 0.05 * vec3(1.0) * albedoColour;

	return ambient + (specular + diffuse) * (pLights[idx].colour * attn);
}

vec3 getSpotLight(int idx)
{
	vec3 lightDir = normalize(sLights[idx].position - posInWs);
	float theta = dot(lightDir, normalize(-sLights[idx].direction));
	float ambientStrength = 0.04;
	vec3 ambient = ambientStrength * sLights[idx].colour;

	if (theta > sLights[idx].outerCutOff)
	{

		float distance = length(sLights[idx].position - posInWs);
		float attn = 1.0 / (sLights[idx].constants.x + (sLights[idx].constants.y * distance) + (sLights[idx].constants.z * (distance * distance)));
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * attn * sLights[idx].colour;

		float specularStrength = 0.8;

		vec3 H = normalize(lightDir + viewDir);
		float specularFactor = pow(max(dot(normal, H), 0.0), 64);
		vec3 specular = pLights[idx].colour * specularFactor * specularStrength;

		float epsilon = sLights[idx].cutOff - sLights[idx].outerCutOff;
		float intensity = clamp((theta - sLights[idx].outerCutOff) / epsilon, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;

		return (ambient + diffuse + specular);
	}
	else
	{
		return ambient;
	}
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;

	float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;

	return a2 / denominator;
}

float GeometrySmith(float NdotV, float NdotL)
{
	float ggx2 = GeometrySchlickGGX(NdotV); //view dir
	float ggx1 = GeometrySchlickGGX(NdotL); //Light dir
	
	return ggx1 * ggx2;
}

float GeometrySchlickGGX(float Ndot)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;
	float denominator = Ndot * (1.0 - k) + k;

	return Ndot / denominator;
}

