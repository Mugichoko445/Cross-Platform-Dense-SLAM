#version 430

// Data structure
struct gMapData
{
	vec4 data;	// Confidence, radius, timestamp, and empty data 
	vec4 vert;	// Vertex
	vec4 norm;	// Normal
	vec4 color;	// Color
};
// Distance global map
layout(std430, binding = 0) buffer gMap
{
	gMapData elems[];
};
uniform mat4 invT;
uniform mat4 P;

flat out int idx;

vec4 transPtForGL(vec4 v)
{
	v = invT * v;
	return P * vec4(v.xy, -v.z, 1.0);
}

void main()
{
	idx = gl_VertexID;
	gl_Position = transPtForGL(elems[idx].vert);
}