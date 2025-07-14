#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;

//Current tonemapping mode
uniform float u_tonemappingMode;

//Defining tonemapping modes
const float TONEMAP_ACES = 0;
const float TONEMAP_UNCHARTED2 = 1;
const float TONEMAP_REINHARD = 2;
const float TONEMAP_FILMIC = 3;
const float TONEMAP_LOTTES = 4;
const float TONEMAP_NEUTRAL = 5;
const float TONEMAP_REINHART2 = 6;
const float TONEMAP_UCHIMURA = 7;
const float TONEMAP_UNREAL = 8;


vec3 aces(vec3 x)
{
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec4 completeAces()
{
	//ACES
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;

	//Tone mapping
	rgb = aces(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	//Ouput final colour
	return vec4(rgb, 1);
}

vec3 uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec4 completeUncharted2()
{
	//Constant whitemap value
	const float W = 11.2;

	//Get input texture
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;

	//Setup exposure
	float exposureBias = 2.0f;
	vec3 curr = uncharted2Tonemap(exposureBias * rgb);
	
	//Whitescale
	vec3 whiteScale = 1.0 / uncharted2Tonemap(vec3(W));

	//Scale by whitescale
	rgb = curr * whiteScale;

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	//Output
	return vec4(rgb, 1.0);
}

vec4 completeReinhard()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb / (1.0 + rgb), 1.0);
}

vec3 filmic(vec3 x)
{
	vec3 X = max(vec3(0.0), x - 0.004);
	vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return pow(result, vec3(2.2));
}

vec4 completeFilmic()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;

	rgb = filmic(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

vec3 lottes(vec3 x)
{
	const vec3 a = vec3(1.6);
	const vec3 d = vec3(0.977);
	const vec3 hdrMax = vec3(8.0);
	const vec3 midIn = vec3(0.18);
	const vec3 midOut = vec3(0.267);

    const vec3 b =
        (-pow(midIn, a) + pow(hdrMax, a) * midOut) /
        ((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);
    const vec3 c =
        (pow(hdrMax, a * d) * pow(midIn, a) - pow(hdrMax, a) * pow(midIn, a * d) * midOut) /
        ((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);

    return pow(x, a) / (pow(x, a * d) * b + c);
}

vec4 completeLottes()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;
	rgb = lottes(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

vec3 neutral (vec3 colour)
{
	const float startCompression = 0.8 - 0.04;
	const float desaturation = 0.15;

	float x = min(colour.r, min(colour.g, colour.b));
	float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
	colour -= offset;

	float peak = max(colour.r, max(colour.g, colour.b));
	if (peak < startCompression) return colour;

	const float d = 1.0 - startCompression;
	float newPeak = 1.0 - d * d / (peak + d - startCompression);

	float g = 1.0 - 1.0 / (desaturation * (peak - newPeak) + 1.0);
	return mix(colour, vec3(newPeak), g);
}

vec4 completeNeutral()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;
	rgb = neutral(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

vec3 reinhart2(vec3 x)
{
	const float L_white = 4.0;

	return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

vec4 completeReinhart2()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;
	rgb = reinhart2(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

vec3 uchimura(vec3 x, float P, float a, float m, float l, float c, float b)
{
  float l0 = ((P - m) * l) / a;
  float L0 = m - m / a;
  float L1 = m + (1.0 - m) / a;
  float S0 = m + l0;
  float S1 = m + a * l0;
  float C2 = (a * P) / (P - S1);
  float CP = -C2 / P;

  vec3 w0 = vec3(1.0 - smoothstep(0.0, m, x));
  vec3 w2 = vec3(step(m + l0, x));
  vec3 w1 = vec3(1.0 - w0 - w2);

  vec3 T = vec3(m * pow(x / m, vec3(c)) + b);
  vec3 S = vec3(P - (P - S1) * exp(CP * (x - S0)));
  vec3 L = vec3(m + a * (x - m));

  return T * w0 + L * w1 + S * w2;
}

vec3 uchimura(vec3 x) 
{
  const float P = 1.0;  // max display brightness
  const float a = 1.0;  // contrast
  const float m = 0.22; // linear section start
  const float l = 0.4;  // linear section length
  const float c = 1.33; // black
  const float b = 0.0;  // pedestal

  return uchimura(x, P, a, m, l, c, b);
}

vec4 completeUchimura()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;
	rgb = uchimura(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

vec3 unreal(vec3 x)
{
	return x / (x + 0.155) * 1.019;
}

vec4 completeUnreal()
{
	vec3 rgb = texture(u_inputTexture, texCoord).rgb;
	rgb = unreal(rgb);

	//Apply gamma correction with gamma 2.2
	rgb = pow(rgb, vec3(1.0 / 2.2));

	//Convert back to linear
	rgb = clamp(rgb, 0.0, 1.0);

	return vec4(rgb, 1.0);
}

void main()
{
	if (u_tonemappingMode == TONEMAP_ACES)
	{
		ppColour = completeAces();
	}
	else if (u_tonemappingMode == TONEMAP_UNCHARTED2)
	{
		ppColour = completeUncharted2();
	}
	else if (u_tonemappingMode == TONEMAP_REINHARD)
	{
		ppColour = completeReinhard();
	}
	else if (u_tonemappingMode == TONEMAP_FILMIC)
	{
		ppColour = completeFilmic();
	}
	else if (u_tonemappingMode == TONEMAP_LOTTES)
	{
		ppColour = completeLottes();
	}
	else if (u_tonemappingMode == TONEMAP_NEUTRAL)
	{
		ppColour = completeNeutral();
	}
	else if (u_tonemappingMode == TONEMAP_REINHART2)
	{
		ppColour = completeReinhart2();
	}
	else if (u_tonemappingMode == TONEMAP_UCHIMURA)
	{
		ppColour = completeUchimura();
	}
	else if (u_tonemappingMode == TONEMAP_UNREAL)
	{
		ppColour = completeUnreal();
	}
	else
	{
		ppColour = completeNeutral();	//Default to neutral
	}
}
