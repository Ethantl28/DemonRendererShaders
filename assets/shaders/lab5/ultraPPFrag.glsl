#version 460 core

layout (location = 0) out vec4 outputColour;
layout (location = 1) out vec4 inFocus;
layout (location = 2) out vec4 outOfFocus;

in vec2 texCoord;

//uniforms
uniform float u_edgeDetectionActive;
uniform float u_blurActive;
uniform float u_vignetteActive;
uniform float u_sepiaActive;

uniform sampler2D u_inputTexture;
uniform vec2 u_screenSize;

uniform float u_blurRadius;
uniform float u_vigInnerRadius;
uniform float u_vigOuterRadius;
uniform float u_vigIntensity;
uniform float u_dofBlurRadius;

//Forward Declarations
void EdgeDetection();
float gaussian(float x, float sigma);
vec3 GaussianBlur(float blurRadius);
void Vignette();
void Sepia();

//Global Variables
vec3 rgb;
float near = 1.0;
float far = 1000.0;
float depthValue;

//Constants
const float PI = 3.14159265359;

void main()
{
	rgb = texture(u_inputTexture, texCoord).rgb;

	//Set up for depth of field next pass
	inFocus = vec4(rgb, 1.0);
	outOfFocus = vec4(GaussianBlur(u_dofBlurRadius), 1.0);

	if (u_edgeDetectionActive == 1.0)
	{
		EdgeDetection();
	}
	if (u_blurActive == 1.0)
	{
		rgb = GaussianBlur(u_blurRadius);
	}
	if (u_vignetteActive == 1.0)
	{
		Vignette();
	}
	if (u_sepiaActive == 1.0)
	{
		Sepia();
	}

	outputColour = vec4(rgb, 1.0);
}

void EdgeDetection()
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
		rgb = vec3(edgeStrength);
	}
	else
	{
		//Draw the background
		rgb = vec3(1.0, 0.0, 0.0);
	}
}

float gaussian(float x, float sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

vec3 GaussianBlur(float blurRadius)
{
	vec2 texelSize = 1.0 / u_screenSize;

        //Gaussian kernel radius
        int kernelRadius = int(blurRadius);
        float sigma = blurRadius / 2.0;

        vec3 result = vec3(0.0);
        float weightSum = 0.0;

        //Combined gaussian blur (horizontal + vertical)
        for (int x = -kernelRadius; x <= kernelRadius; ++x)
        {
            for (int y = -kernelRadius; y <= kernelRadius; ++y)
            {
                //Distance from center
                float distance = length(vec2(float(x), float(y)));

                //Compute gaussian weight
                float weight = gaussian(distance, sigma);

                //Offset for sampling
                vec2 offset = vec2(float(x), float(y)) * texelSize;

                //Sample the color from the texture
                vec3 sampleCol = texture(u_inputTexture, texCoord + offset).rgb;

                //Accumulate weighted color
                result += sampleCol * weight;
                weightSum += weight;
            }
        }

        //Normalize the result by the weight sum
        result /= weightSum;

        //Output the final color
        return result;
}

void Vignette()
{
	//Centre of screen in screen space
	vec2 centreScreen = vec2(0.5);
	
	//Distance from centre
	float dist = distance(centreScreen, texCoord);

	//Smoothstep for vignette
	float vignette = smoothstep(u_vigOuterRadius, u_vigInnerRadius, dist);

	//Apply vignette
	rgb *= mix(1.0, vignette, u_vigIntensity);
}

void Sepia()
{
	vec3 sepia;
	sepia.r = dot(rgb, vec3(0.393, 0.769, 0.189));
	sepia.g = dot(rgb, vec3(0.349, 0.686, 0.168));
	sepia.b = dot(rgb, vec3(0.272, 0.534, 0.131));
	
	rgb = sepia;
}