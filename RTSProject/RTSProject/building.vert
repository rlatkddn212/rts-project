#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 mapuv;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 worldMatrix;
uniform mat4 vpMatrix;
uniform vec3 lightDir;

void main()
{
	vec4 pos = worldMatrix * vec4(inPosition.xyz, 1.0);
	
	// ¸Ê ÁÂÇ¥
	mapuv = vec2(pos.x / 100.0, -pos.z / 100.0);
	
	fragNormal = (worldMatrix * vec4(inNormal, 1.0)).xyz;
	fragTexCoord = inTexCoord;
	fragWorldPos = pos.xyz;

	gl_Position = vpMatrix * pos;
}