#version 420

in vec3 vsVert;
in vec3 vsNorm;

void main()
{
	gl_FragData[0] = vec4(vsVert, 1.0);
	gl_FragData[1] = vec4(vsNorm, 1.0);
}