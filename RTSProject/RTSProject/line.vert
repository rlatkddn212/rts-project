#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
out vec4 col;

void main(void)
{
    col = color;
	gl_Position = vec4(position.xyz, 1.0); 
}
