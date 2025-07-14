#version 460 core

layout(location = 0) out vec4 dofColour;

in vec2 texCoord;

// Uniforms
uniform sampler2D u_inFocus;       
uniform sampler2D u_outOfFocus;    
uniform sampler2D u_depthTexture;  
uniform float u_active;            
uniform float u_focalDistance;     // Focal plane distance
uniform float u_focalRange;        // Range around the focal plane

float near = 1.0;
float far = 1000.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    if (u_active == 1.0)
    {
        float depthValue = texture(u_depthTexture, texCoord).r;

        float linearDepth = LinearizeDepth(depthValue);
        float depthNormalized = (linearDepth - near) / (far - near);

        // Correct the depth normalization range.
        float depthDifference = abs(depthNormalized - (u_focalDistance - near) / (far - near));
        float focusFactor = clamp(1.0 - (depthDifference / (u_focalRange / (far - near))), 0.0, 1.0);

        vec3 colourInFocus = texture(u_inFocus, texCoord).rgb;
        vec3 colourOutOfFocus = texture(u_outOfFocus, texCoord).rgb;

        vec3 mixedColour = mix(colourOutOfFocus, colourInFocus, focusFactor);

        dofColour = vec4(mixedColour, 1.0);
    }
    else
    {
        vec3 rgb = texture(u_inFocus, texCoord).rgb;
        dofColour = vec4(rgb, 1.0);
    }
}
