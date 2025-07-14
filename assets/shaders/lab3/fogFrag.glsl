#version 460 core

layout(location = 0) out vec4 depthColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;
uniform sampler2D u_depthTexture;
uniform float u_active;
uniform vec3 u_fogColour;
uniform float u_fogDensity;

float far = 1000.0;
float near = 1.0;

float LineariseDepth(float depth)
{
	float z = depth * 2.0 - 1.0;	//back to clip space
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	if (u_active == 1.0)
	{
		//Get depth from input texture
		float depthValue = texture(u_depthTexture, texCoord).r;

		//Get colour of fragment
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;

		//linearise depth
		float linearDepth = LineariseDepth(depthValue);

		//Fog factor exponential
		float fogFactor = 1.0 - exp(-u_fogDensity * linearDepth);

		//Clamp for safety
		fogFactor = clamp(fogFactor, 0.0, 1.0);

		//normalise depth
		float depthNormalised = (linearDepth - near) / (far - near);

		//Mix colour, fogColour, and depth
		vec3 mixedColour = mix(rgb, u_fogColour, fogFactor);

		//Return output colour
		depthColour = vec4(mixedColour, 1.0);
	}
	else
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		depthColour = vec4(rgb, 1.0);
	}
}
