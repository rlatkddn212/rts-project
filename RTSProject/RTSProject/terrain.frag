

#version 430 core

layout(binding = 0) uniform sampler2D image01;
layout (binding = 1) uniform sampler2D image02;
layout (binding = 2) uniform sampler2D image03;
layout (binding = 3) uniform sampler2D image04;
layout (binding = 4) uniform sampler2D image05;

in vec2 fragTexCoord;
in vec2 fragAlphaCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

void main()
{
	vec4 a = texture(image04, fragAlphaCoord);
	vec4 c1 = texture(image01, fragTexCoord);
	vec4 c2 = texture(image02, fragTexCoord);
	vec4 c3 = texture(image03, fragTexCoord);
	vec4 c4 = texture(image05, fragAlphaCoord);
	
	float inverse = 1.0f / (a.r + a.g + a.b);

	c1 *= a.r * inverse;
    c2 *= a.g * inverse;
    c3 *= a.b * inverse;

	outDiffuse = ((c1 + c2 + c3) * c4).xyz;
	outNormal=  fragNormal;
	outWorldPos = fragWorldPos;
}