#version 460 core

layout(vertices = 3) out;

in vec3 vs_fragmentPos[];
in vec2 vs_texCoord[];

out vec3 tcs_fragmentPos[];
out vec2 tcs_texCoord[];

uniform vec3 u_viewPosTCS;

float GetTessLevel(float distance0, float distance1);

void main()
{
	float eyetoVertexDist0 = distance(u_viewPosTCS, vs_fragmentPos[0]);
	float eyetoVertexDist1 = distance(u_viewPosTCS, vs_fragmentPos[1]);
	float eyetoVertexDist2 = distance(u_viewPosTCS, vs_fragmentPos[2]);

	if (gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = GetTessLevel(eyetoVertexDist1, eyetoVertexDist2);
		gl_TessLevelOuter[1] = GetTessLevel(eyetoVertexDist2, eyetoVertexDist0);
		gl_TessLevelOuter[2] = GetTessLevel(eyetoVertexDist0, eyetoVertexDist1);
		gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}

	tcs_fragmentPos[gl_InvocationID] = vs_fragmentPos[gl_InvocationID];
	tcs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];
}

float GetTessLevel(float distance0, float distance1)
{
	float avgDistance = (distance0 + distance1) / 2.0;

	return exp(-avgDistance / 25.0) * 64.0;
}