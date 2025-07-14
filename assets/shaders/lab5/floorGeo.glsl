#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tes_fragmentPos[];
in vec2 tes_texCoord[];

out vec3 geo_fragmentPos;
out vec2 geo_texCoord;

vec3 getNormal();

void main()
{
	for (int i = 0; i < 3; i++)
	{
		geo_texCoord = tes_texCoord[i];
		geo_fragmentPos = tes_fragmentPos[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}