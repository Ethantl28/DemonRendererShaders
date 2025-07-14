#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;
uniform float u_active;
uniform vec2 u_screenSize;

void main()
{
	if (u_active == 1.0)
	{
		vec2 inverseScreen = 1.0 / u_screenSize;

		vec3 colourCentre = texture(u_inputTexture, texCoord).rgb;
		vec3 colourLeft = texture(u_inputTexture, texCoord + vec2(-1.0, 0.0) * inverseScreen).rgb;
		vec3 colourRight = texture(u_inputTexture, texCoord + vec2(1.0, 0.0) * inverseScreen).rgb;
		vec3 colourAbove = texture(u_inputTexture, texCoord + vec2(0.0, -1.0) * inverseScreen).rgb;
		vec3 colourBelow = texture(u_inputTexture, texCoord + vec2(0.0, 1.0) * inverseScreen).rgb;

		//Luminance values to detect edges
		float lumCentre = dot(colourCentre, vec3(0.2126, 0.7152, 0.0722));
		float lumLeft = dot(colourLeft, vec3(0.2126, 0.7152, 0.0722));
		float lumRight = dot(colourRight, vec3(0.2126, 0.7152, 0.0722));
		float lumAbove = dot(colourAbove, vec3(0.2126, 0.7152, 0.0722));
		float lumBelow = dot(colourBelow, vec3(0.2126, 0.7152, 0.0722));

		//Determine strength of edge
		float edgeHorizontal = abs(lumLeft - lumRight);
		float edgeVertical = abs(lumAbove - lumBelow);
		float edgeStrength = max(edgeHorizontal, edgeVertical);

		if (edgeStrength > 0.1)
		{
			//Draw the edge
			ppColour = vec4(vec3(edgeStrength), 1.0);
		}
		else
		{
			//Draw the background
			ppColour = vec4(vec3(1.0, 0.0, 0.0), 1.0);
		}
	}
	else	
	{
		vec3 rgb = texture(u_inputTexture, texCoord).rgb;
		ppColour = vec4(rgb, 1.0);
	}
}
