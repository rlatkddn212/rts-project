#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 mvp_matrix;

out vec4 out_color;

void main()
{
	gl_Position = vec4(position.xyz, 1.0) * mvp_matrix;

	out_color = color;
}
