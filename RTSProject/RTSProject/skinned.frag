#version 430 core

layout(binding = 0) uniform sampler2D image01;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

uniform vec4 teamColor;

void main()
{
	vec4 col = texture(image01, fragTexCoord);
	float lnv = 1.0f - col.a;
	vec4 c1 = vec4(col.rgb * lnv + teamColor.rgb * col.a, 1.0f);

	outDiffuse = c1.xyz;
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}