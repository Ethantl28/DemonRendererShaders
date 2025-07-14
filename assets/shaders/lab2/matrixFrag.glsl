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
		rgb = pow(rgb, vec3(1.5, 0.8, 1.5));
		ppColour = vec4(rgb, 1.0);
	}
	else	
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		ppColour = vec4(rgb, 1.0);
	}
}
