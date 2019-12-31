#version 430 core

layout(binding = 0) uniform sampler2D image01;


in vec2 fragTexCoord;
in float shade;
out vec4 color;

uniform vec4 teamColor;

void main()
{
	vec4 col = texture(image01, fragTexCoord);
	float lnv = 1.0f - col.a;
	vec4 c1 = vec4(col.rgb * lnv + teamColor.rgb * col.a, 1.0f);

	color = c1 * shade;
}