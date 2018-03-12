#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

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

uniform mat4 invT;	// Transformation from the world to the sensor
uniform mat4 P;
uniform float c_stable;	// points with "c >= c_stable" are considered stable
uniform float scale;		// enlarge surfels to reduce noisy dots...

flat in int index[];

out vec3 gsVert;
out vec3 gsNorm;
out vec3 gsColor;
out vec2 gsUvTex;

vec4 transPtForGL(vec4 v)
{
	v = invT * v;
	return P * vec4(v.xy, -v.z, 1.0);
}

void main(void)
{
	int idx = index[0];
	float rad = elems[idx].data.y;
	float conf = elems[idx].data.x;

	if (rad > 0.0 && conf >= c_stable)
	{
		vec3 norm = elems[idx].norm.xyz;
		vec4 vert = gl_in[0].gl_Position;
		vec4 cPos = transPtForGL(vert);
		cPos /= cPos.w;
		
		if (cPos.x >= -1.0 && cPos.x <= 1.0 && cPos.y >= -1.0 && cPos.y <= 1.0 && cPos.z > 0.0 && cPos.z < 1.0)
		{
			gsVert = mat4x3(invT) * vert;
			gsNorm = mat3(invT) * norm;
			gsColor = elems[idx].color.rgb;
			
			// Create tangent space.
			vec3 v = abs(norm.y) > abs(norm.x) ? cross(norm, vec3(1.0, 0.0, 0.0)) : cross(vec3(0.0, 1.0, 0.0), norm);
			vec3 u = cross(norm, v);
			// Scale to radius
			u *= rad * scale;
			v *= rad * scale;
			// Transform and emit four corners
			gl_Position = transPtForGL(vec4(vert.xyz - u + v, 1.0));
			gsUvTex = vec2(-1.0, 1.0);
			EmitVertex();
			gl_Position = transPtForGL(vec4(vert.xyz - u - v, 1.0));
			gsUvTex = vec2(-1.0,-1.0);
			EmitVertex();
			gl_Position = transPtForGL(vec4(vert.xyz + u + v, 1.0));
			gsUvTex = vec2( 1.0, 1.0);
			EmitVertex();
			gl_Position = transPtForGL(vec4(vert.xyz + u - v, 1.0));
			gsUvTex = vec2( 1.0,-1.0);
			EmitVertex();
		}
	}
}