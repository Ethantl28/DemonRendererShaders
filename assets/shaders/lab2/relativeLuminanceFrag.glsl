#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;
uniform float u_active;

void main()
{
	if (u_active == 1.0)
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		float luminance = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
		ppColour = vec4(vec3(luminance), 1.0);
	}
	else	
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		ppColour = vec4(rgb, 1.0);
	}
}
