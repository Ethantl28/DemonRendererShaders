#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(std140, binding = 1) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

in vec3 vs_fragmentPos[];

out vec2 texCoord;
out vec3 fragmentPos;
out vec3 vertexNormal;

mat4 VP;
float size = 5;

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(u_viewPos - pos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(toCamera, up));
	vec3 toCam = normalize(u_viewPos - pos);

	//Corners
	vec3 corner0 = pos - right * size - up * size; //bottom left
	vec3 corner1 = pos + right * size - up * size; //bottom right
	vec3 corner2 = pos - right * size + up * size; //top left
	vec3 corner3 = pos + right * size + up * size; //top right

	//Normal
	vertexNormal = toCam;

	VP = u_projection * u_view;

	//Emit
	texCoord = vec2(0.0, 0.0); //bottom left
	fragmentPos = corner0;
	gl_Position = VP * vec4(corner0, 1.0);
	EmitVertex();

	texCoord = vec2(1.0, 0.0); //bottom right
	fragmentPos = corner1;
	gl_Position = VP * vec4(corner1, 1.0);
	EmitVertex();

	texCoord = vec2(0.0, 1.0); //top left
	fragmentPos = corner2;
	gl_Position = VP * vec4(corner2, 1.0);
	EmitVertex();

	texCoord = vec2(1.0, 1.0); //top right
	fragmentPos = corner3;
	gl_Position = VP * vec4(corner3, 1.0);
	EmitVertex();


	EndPrimitive();
}