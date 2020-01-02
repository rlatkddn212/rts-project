#version 420
in vec2 outtc;
out vec4 outColor;
layout (binding = 0) uniform sampler2D mapTexture;
layout (binding = 1) uniform sampler2D fogTexture;
layout (binding = 2) uniform sampler2D posTexture;

void main()
{
	vec4 col = texture(posTexture, outtc);
	if (col.g <= 0)
	{
		outColor = texture(mapTexture, outtc) * texture(fogTexture, outtc);
	}
	else
	{
		outColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
}
