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

		vec3 sepia;
		sepia.r = dot(rgb, vec3(0.393, 0.769, 0.189));
		sepia.g = dot(rgb, vec3(0.349, 0.686, 0.168));
		sepia.b = dot(rgb, vec3(0.272, 0.534, 0.131));
		
		ppColour = vec4(sepia, 1.0);
	}
	else	
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		ppColour = vec4(rgb, 1.0);
	}
}
