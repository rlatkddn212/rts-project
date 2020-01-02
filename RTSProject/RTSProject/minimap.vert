#version 420 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 tc;
out vec2 outtc;

uniform mat4 mMatrix;

void main(void)
{
    outtc = tc;
	gl_Position = mMatrix * vec4(position.xyz, 1.0); 
}
