#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoord;

uniform sampler2D u_inputTexture;
uniform float u_active;
uniform float u_blurRadius;
uniform vec2 u_screenSize;

const float PI = 3.14159265359;

float gaussian(float x, float sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

void main()
{
    if (u_active == 1.0)
    {
        vec2 texelSize = 1.0 / u_screenSize;

        // Gaussian kernel radius (convert float to int for loop)
        int kernelRadius = int(u_blurRadius);
        float sigma = u_blurRadius / 2.0;

        vec3 result = vec3(0.0);
        float weightSum = 0.0;

        // Combined Gaussian blur (horizontal + vertical)
        for (int x = -kernelRadius; x <= kernelRadius; ++x)
        {
            for (int y = -kernelRadius; y <= kernelRadius; ++y)
            {
                // Distance from center
                float distance = length(vec2(float(x), float(y)));

                // Compute Gaussian weight
                float weight = gaussian(distance, sigma);

                // Offset for sampling
                vec2 offset = vec2(float(x), float(y)) * texelSize;

                // Sample the color from the texture
                vec3 sampleCol = texture(u_inputTexture, texCoord + offset).rgb;

                // Accumulate weighted color
                result += sampleCol * weight;
                weightSum += weight;
            }
        }

        // Normalize the result by the weight sum
        result /= weightSum;

        // Output the final color
        ppColour = vec4(result, 1.0);
    }
    else
    {
        // If not active, just output the original texture color
        vec3 rgb = texture(u_inputTexture, texCoord).rgb;
        ppColour = vec4(rgb, 1.0);
    }
}
