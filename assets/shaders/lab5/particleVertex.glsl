#version 460 core
layout(location = 0) in vec3 inPosition;

struct Particle
{
    vec4 Position;
    vec4 Velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer
{
    vec4 data[];
};

out vec4 vs_worldPosition;

void main()
{
    vs_worldPosition = data[gl_VertexID * 2];

    gl_PointSize = 5.0;
}
