#version 430 core

layout(binding = 0) uniform sampler2D image01;
layout(binding = 1) uniform sampler2D image02;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;
in vec2 mapuv;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

void main()
{
	outDiffuse = (texture(image01, fragTexCoord) * texture(image02, mapuv)).xyz;
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}
