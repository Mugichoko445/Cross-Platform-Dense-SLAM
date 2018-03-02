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

flat out int index;

void main(void)
{
	index = gl_VertexID;
	gl_Position = vec4(elems[index].vert.xyz, 1.0);
}