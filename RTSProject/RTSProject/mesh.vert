#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out float shade;
out vec2 fragTexCoord;
out vec2 mapuv;

uniform mat4 worldMatrix;
uniform mat4 vpMatrix;
uniform vec3 lightDir;

void main()
{
	fragTexCoord = inTexCoord;
	float diff = max(0.0f, dot(normalize(inNormal), lightDir));
	shade = 0.2f + diff;

	vec4 pos = worldMatrix * vec4(inPosition.xyz, 1.0);
	mapuv = vec2(pos.x / 100.0, -pos.z / 100.0);
	
	gl_Position = vpMatrix * pos;
}