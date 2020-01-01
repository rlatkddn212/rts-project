#version 420 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 tc;
layout(location = 2) in vec3 offset;

uniform mat4 vp_matrix;

out vec2 outtc;

void main(void)
{
    outtc = tc;
	 vec4 pos = vec4(10 * position.xzy + offset, 1.0); 
	 gl_Position = vp_matrix * pos;
}
