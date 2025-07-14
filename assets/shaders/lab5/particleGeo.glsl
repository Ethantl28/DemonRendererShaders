#version 460 core

layout(points) in;
layout(points, max_vertices = 1) out;

layout(std140, binding = 1) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

in vec4 vs_worldPosition[];

out float geo_distanceFromCam;

uniform float u_active;

void main()
{
	geo_distanceFromCam = distance(vs_worldPosition[0].xyz, u_viewPos);

	if (geo_distanceFromCam > 50.0 || u_active < 1.0)
	{
		
	}
	else
	{
		gl_Position = (u_projection * u_view) * vs_worldPosition[0];
		EmitVertex();
	}

	EndPrimitive();
}