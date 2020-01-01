#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 vpMatrix;

out vec4 out_color;

void main()
{
	gl_Position = vpMatrix * vec4(position.xyz, 1.0);

	out_color = color;
}
