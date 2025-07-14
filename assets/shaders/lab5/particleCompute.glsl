#version 460
layout (local_size_x = 256) in; 

struct Particle {
    vec4 Position;
    vec4 Velocity;
};

layout (std430, binding = 0) buffer ParticleBuffer {
    vec4 data[];
};

uniform float deltaTime;
const uint PARTICLE_VEC4_COUNT = 2; //pos, vel and size
const uint TOTAL_PARTICLES = 1280000;

void main() {
    uint id = gl_GlobalInvocationID.x;
    if (id >= TOTAL_PARTICLES) return;

    //compute index positions
    uint posIndex = id * PARTICLE_VEC4_COUNT;
    uint velIndex = posIndex + 1;
    uint randomPosIndex = id + TOTAL_PARTICLES * 2;

    //read pos and vel
    vec4 position = data[posIndex];
    vec4 velocity = data[velIndex];

    //update position
    position.xyz += velocity.xyz * deltaTime;

    //reset if below ground
    if (position.y <= -5.0)
    {
        position = data[randomPosIndex];
    }

    //write back to ssbo
    data[posIndex] = position;
}
