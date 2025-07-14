#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 u_model;

out vec3 vs_fragmentPos;

void main()
{
	vs_fragmentPos = vec3(u_model * vec4(aPos, 1.0));
	gl_Position = u_model * vec4(aPos, 1.0);
}