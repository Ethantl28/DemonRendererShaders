#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;
uniform float u_active;
uniform float u_innerRadius;
uniform float u_outerRadius;

void main()
{
	if (u_active == 1.0)
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;

		//Centre of screen in screen space
		vec2 centreScreen = vec2(0.5);
		
		//Distance from centre
		float dist = distance(centreScreen, texCoord);

		//Smoothstep for vignette
		float vignette = smoothstep(u_innerRadius, u_outerRadius, dist);

		//Apply vignette
		ppColour = vec4(rgb * (1.0 - vignette), 1.0);
	}
	else	
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		ppColour = vec4(rgb, 1.0);
	}
}
