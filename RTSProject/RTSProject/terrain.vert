#version 420 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec2 alphaCoord;

out vec2 fragTexCoord;
out vec2 fragAlphaCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 vpMatrix;

void main()
{
	fragTexCoord = inTexCoord;
	fragAlphaCoord = alphaCoord;
	fragNormal = inNormal;
	fragWorldPos = inPosition;
	gl_Position = vpMatrix * vec4(inPosition.xyz, 1.0);
}
