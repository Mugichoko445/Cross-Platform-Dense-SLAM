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

flat out vec3 vsVert;
flat out vec3 vsNorm;
flat out vec3 vsColor;
flat out float vsRadius;
flat out float vsConf;

void main(void)
{
	int idx = gl_VertexID;
	vsConf = elems[idx].data.x;
	vsRadius = elems[idx].data.y;
	vsNorm = elems[idx].norm.xyz;
	vsVert = elems[idx].vert.xyz;
	vsColor = elems[idx].color.rgb;
	gl_Position = vec4(vsVert, 1.0);
}