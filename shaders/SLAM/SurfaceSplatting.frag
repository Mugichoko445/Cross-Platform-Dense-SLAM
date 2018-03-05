#version 430

in vec3 gsVert;
in vec3 gsNorm;
in vec3 gsColor;
in vec2 gsUvTex;

void main(void)
{
	// this pixel-wise process does not contribute much
	// since the surfels are very small
	//float val = length(gsUvTex);
	//if (val > 1.0) discard;

	gl_FragData[0] = vec4(gsVert, 1.0);
	gl_FragData[1] = vec4(gsNorm, 0.0);
	gl_FragData[2] = vec4(gsVert.zzz, 0.0);
	gl_FragData[3] = vec4(gsColor, 1.0);
}