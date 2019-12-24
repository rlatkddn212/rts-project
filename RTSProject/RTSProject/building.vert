#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out float shade;
out vec2 fragTexCoord;

uniform mat4 mvp_matrix;
uniform vec3 lightDir;

void main()
{
	fragTexCoord = inTexCoord;

	float diff  = max(0.0f, dot(normalize(inNormal), lightDir));
	shade = 0.2f + diff;

	gl_Position = mvp_matrix * vec4(inPosition.xyz, 1.0);
}
