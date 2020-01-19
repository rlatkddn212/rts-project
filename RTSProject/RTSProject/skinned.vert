#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in ivec4 inBoneIDs;
layout(location = 3) in vec4 inWeights;
layout(location = 4) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 gBones[100];
uniform mat4 vpMatrix;
uniform mat4 worldMatrix;
uniform vec3 lightDir;

void main()
{
	mat4 BoneTransform = gBones[inBoneIDs[0]] * inWeights[0];
    BoneTransform     += gBones[inBoneIDs[1]] * inWeights[1];
    BoneTransform     += gBones[inBoneIDs[2]] * inWeights[2];
    BoneTransform     += gBones[inBoneIDs[3]] * inWeights[3];

	vec4 skinnedPos = worldMatrix * BoneTransform * vec4(inPosition, 1.0);
	
	// Send to fragShader
	fragWorldPos = skinnedPos.xyz;
	// normal은 postion이 아니므로 0.0으로 해야한다.
	fragNormal = ((worldMatrix * BoneTransform * vec4(inNormal, 0.0)).xyz);
	fragTexCoord = inTexCoord;

	gl_Position = vpMatrix * skinnedPos;
}
