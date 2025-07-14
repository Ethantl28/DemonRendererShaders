#version 460 core

layout(location = 0) out vec4 colour;


in vec3 vertexNormal;
in vec3 fragmentPos;
in vec2 texCoord;


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

const int numPointLights = 1;
const int numSpotLights = 1;

layout (std140, binding = 1) uniform b_lights
{
	uniform directionalLight dLight;
	uniform pointLight pLights[numPointLights];
	uniform spotLight sLights[numSpotLights];
};

layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};


uniform sampler2D u_albedoMap;
uniform vec3 u_albedo;



vec3 getDirectionalLight() ;
vec3 getPointLight(int idx) ;
vec3 getSpotLight(int idx) ;


//global vars
vec3 normal ;
vec3 albedoColour ;
float specularStrength  = 0.8 ;
vec3 viewDir ;


void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0); 
    viewDir = normalize(u_viewPos - fragmentPos);
	normal = normalize(vertexNormal) ;
	albedoColour = texture(u_albedoMap, texCoord).rgb;



	// light casters

	result += getDirectionalLight();
	
	for(int i = 0; i <numPointLights; i++)
	{
		//result += getPointLight(i);
	}
	
	for(int i = 0; i <numSpotLights; i++)
	{
		//result += getSpotLight(i);
	}
	      
	colour = vec4(result * u_albedo, 1.0) * texture(u_albedoMap, texCoord);
}


vec3 getDirectionalLight()
{
	float ambientStrength = 0.9;
	vec3 ambient = ambientStrength * dLight.colour ;

	float diffuseFactor = max(dot(normal, -dLight.direction), 0.0);
	vec3 diffuse = diffuseFactor * dLight.colour ;
	
	vec3 H = normalize(-dLight.direction + viewDir ) ;
	float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
    vec3 specular = dLight.colour * specularFactor * specularStrength;

	return specular + (diffuse + specular);
}

vec3 getPointLight(int idx)
{
	float ambientStrength = 0.04;
	vec3 ambient = ambientStrength * pLights[idx].colour;

	float distance = length(pLights[idx].position - fragmentPos);
	float attn = 1.0/(pLights[idx].constants.x + (pLights[idx].constants.y* distance) + (pLights[idx].constants.z * (distance * distance)));
	vec3 lightDir = normalize(pLights[idx].position - fragmentPos);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * attn * pLights[idx].colour;

	vec3 H = normalize(lightDir + viewDir ) ;
	float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
    vec3 specular = pLights[idx].colour * specularFactor * specularStrength;
	
	return ambient + diffuse + specular;
}

vec3 getSpotLight(int idx)
{	
	vec3 lightDir = normalize(sLights[idx].position - fragmentPos);
	float theta = dot(lightDir, normalize(-sLights[idx].direction));
	float ambientStrength = 0.04 ;
	vec3 ambient = ambientStrength * sLights[idx].colour;

	if(theta > sLights[idx].outerCutOff)
	{

		float distance = length(sLights[idx].position - fragmentPos);
		float attn = 1.0/(sLights[idx].constants.x + (sLights[idx].constants.y* distance) + (sLights[idx].constants.z * (distance * distance)));
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * attn * sLights[idx].colour;

		float specularStrength = 0.8;

		vec3 H = normalize(lightDir + viewDir ) ;
	    float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
        vec3 specular = pLights[idx].colour * specularFactor * specularStrength; 
	
		float epsilon = sLights[idx].cutOff - sLights[idx].outerCutOff;
		float intensity = clamp((theta - sLights[idx].outerCutOff) / epsilon, 0.0, 1.0); 
	
		diffuse  *= intensity;
		specular *= intensity;
		
		return (ambient + diffuse + specular);
	}
	else
	{
		return ambient;
	}
}

