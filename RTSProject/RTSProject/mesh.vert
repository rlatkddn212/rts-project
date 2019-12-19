#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;

uniform mat4 mvp_matrix;

void main()
{
	fragTexCoord = inTexCoord;
	gl_Position = mvp_matrix * vec4(inPosition.xyz, 1.0);
}
