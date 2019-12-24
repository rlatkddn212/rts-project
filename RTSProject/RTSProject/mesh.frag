#version 430 core

layout(binding = 0) uniform sampler2D image01;
in vec2 fragTexCoord;
out vec4 color;
in float shade;

void main()
{
	color = texture(image01, fragTexCoord);
}