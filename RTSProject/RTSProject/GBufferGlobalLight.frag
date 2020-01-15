#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
out vec4 col;

uniform mat4 mMatrix;

void main(void)
{
    col = color;
	gl_Position = mMatrix * vec4(position.xyz, 1.0); 
}
