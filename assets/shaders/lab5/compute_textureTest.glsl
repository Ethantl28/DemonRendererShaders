#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform image2D noiseMapImg;
layout(binding = 1, rgba8) uniform image2D perVertexNormalImg;

uniform float u_heightMapScale;

uniform float u_frequency;
uniform float u_amplitude;
uniform float u_octaves;
uniform float u_lacunarity;
uniform float u_persitance;

vec2 heightMapSize = vec2(512, 512);

float PI = 3.1415;

vec2 uv;

//Forward Declarations
float remap(float currValue, float  inMin,float inMax,float outMin, float outMax);
vec2 hash2(vec2 p);
float noise(in vec2 p);
float FBM(vec2 position);

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	uv = vec2(pixel_coords) / vec2(heightMapSize);
	vec4 pixelCol = vec4(0.0);

	float noiseValue = FBM(uv);
	noiseValue = remap(noiseValue, -1.0, 1.0, 0.0, 1.0);

	pixelCol = vec4(vec3(noiseValue), 1.0);

	imageStore(noiseMapImg, pixel_coords, pixelCol);
	vec2 texelSize = 1.0 / heightMapSize;

	float right = FBM(uv + vec2(texelSize.x, 0.0)) * u_heightMapScale;
	float left = FBM(uv - vec2(texelSize.x, 0.0)) * u_heightMapScale;
	float up = FBM(uv - vec2(0.0, texelSize.y)) * u_heightMapScale;
	float down = FBM(uv + vec2(0.0, texelSize.y)) * u_heightMapScale;
	
	float lr = left - right;
	float du = up - down;
	vec3 normal = normalize(vec3(lr, 2.0, du));

	pixelCol = vec4(vec3(normal), 1.0);

	imageStore(perVertexNormalImg, pixel_coords, pixelCol);
}

float remap(float currValue, float  inMin,float inMax,float outMin, float outMax)
{
	
	float t = (currValue - inMin) / (inMax - inMin);
	return mix(outMin, outMax, t);
}

vec2 hash2(vec2 p)
{
	p = vec2(dot(p, vec2(127.1, 311.7)),
		dot(p, vec2(269.5, 183.3)));

	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123 );
}

float noise(in vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
		dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
		mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

float FBM(vec2 position)
{
	float total = 0.0;
	float amplitude = u_amplitude;
	float frequency = u_frequency;
	float maxAmplitude = 0.0;

	for (int i = 0; i < u_octaves; i++)
	{
		total += noise(position * frequency) * amplitude * u_amplitude;
		frequency *= u_lacunarity;
		amplitude *= u_persitance;
		maxAmplitude += amplitude;
	}

	return total / maxAmplitude;
}