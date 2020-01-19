#version 420 core

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

uniform sampler2D uTexture;

void main()
{
	outDiffuse = texture(uTexture, fragTexCoord).xyz;
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}