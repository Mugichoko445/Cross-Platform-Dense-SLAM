#version 430

flat in int idx;

void main()
{
	gl_FragColor = vec4(float(idx));
}