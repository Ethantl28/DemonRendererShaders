#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;

float far = 1000.0;
float near = 1.0;

float LineariseDepth(float depth)
{
	float z = depth * 2.0 - 1.0;	//back to clip space
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	//Get depth from depth texture
	float depthValue = texture(u_inputTexture, texCoord).r;

	//linearise and normalise depth
	float linearDepth = LineariseDepth(depthValue);

	//Return output colour
	ppColour = vec4(vec3(depthValue), 1.0);
}
