#version 440 core

layout (binding = 0) uniform sampler2D tex;

in vec2 vsTexCoord;

uniform int mapType;
uniform float maxDepth;

void main()
{
	if (mapType == 0)		// BGR image
	{
		gl_FragColor = texture(tex, vsTexCoord).bgra;
	}
	else if (mapType == 1)	// RGB image
	{
		gl_FragColor = texture(tex, vsTexCoord).rgba;
	}
	else if (mapType == 2)	// Depth map
	{
		gl_FragColor = vec4(texture(tex, vsTexCoord).rrr / maxDepth, 1.0);
	}
	else if (mapType == 4)	// Normal map
	{
		gl_FragColor = abs(texture(tex, vsTexCoord).bgra);
	}
}