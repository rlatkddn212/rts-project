#version 430 core

layout(binding = 0) uniform sampler2D image01;
layout(binding = 1) uniform sampler2D image02;
in vec2 fragTexCoord;
in vec2 mapuv;
in float shade;

out vec4 color;

void main()
{
	color = texture(image01, fragTexCoord) * texture(image02, mapuv);
}