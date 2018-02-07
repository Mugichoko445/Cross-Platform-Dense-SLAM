#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 invT;	// Transformation from the world to the sensor
uniform mat4 P;

const float PI = 3.1415927;
uniform float c_stable;	// points with "c >= c_stable" are considered stable


flat in vec3 vsVert[];
flat in vec3 vsNorm[];
flat in vec3 vsColor[];
flat in float vsRadius[];
flat in float vsConf[];

out vec3 gsVert;
out vec3 gsNorm;
out vec3 gsColor;
out vec2 gsUvTex;

vec4 transPtForGL(vec4 v)
{
	v = invT * v;
	return P * vec4(v.xy, -v.z, 1.0);
}
vec3 rotPtForGL(vec3 v)
{
	v = mat3(invT) * v;
	return vec3(v.xy, -v.z);
}

void main(void)
{
	if (vsRadius[0] > 0.0 && vsConf[0] >= c_stable)
	{
		vec4 cPos = transPtForGL(gl_in[0].gl_Position);
		cPos /= cPos.w;
		
		if (cPos.x >= -1.0 && cPos.x <= 1.0 && cPos.y >= -1.0 && cPos.y <= 1.0 && cPos.z > 0.0 && cPos.z < 1.0)
		{
			gsVert = mat4x3(invT) * vec4(vsVert[0].xyz, 1.0);
			gsNorm = mat3(invT) * vsNorm[0];
			gsColor = vsColor[0];
			
			// Create tangent space.
			vec3 v = abs(vsNorm[0].y) > abs(vsNorm[0].x) ? cross(vsNorm[0], vec3(1.0, 0.0, 0.0)) : cross(vec3(0.0, 1.0, 0.0), vsNorm[0]);
			vec3 u = cross(vsNorm[0], v);
			// Scale to radius
			u *= vsRadius[0];
			v *= vsRadius[0];
			// Transform and emit four corners
			gl_Position = transPtForGL(gl_in[0].gl_Position + vec4(-u +v, 0.0));
			gsUvTex = vec2(-1.0, 1.0);
			EmitVertex();
			gl_Position = transPtForGL(gl_in[0].gl_Position + vec4(-u -v, 0.0));
			gsUvTex = vec2(-1.0,-1.0);
			EmitVertex();
			gl_Position = transPtForGL(gl_in[0].gl_Position + vec4(+u +v, 0.0));
			gsUvTex = vec2( 1.0, 1.0);
			EmitVertex();
			gl_Position = transPtForGL(gl_in[0].gl_Position + vec4(+u -v, 0.0));
			gsUvTex = vec2( 1.0,-1.0);
			EmitVertex();
		}
	}
}