#version 420 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
out vec4 col;
uniform mat4 mvp_matrix;

void main(void)
{
    col = color;
	gl_Position = mvp_matrix * vec4(position.xyz, 1.0); 
}
