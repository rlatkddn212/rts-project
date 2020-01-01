#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in ivec4 inBoneIDs;
layout(location = 3) in vec4 inWeights;
layout(location = 4) in vec2 inTexCoord;

out vec2 fragTexCoord;
out float shade;

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

	fragTexCoord = inTexCoord;
	shade = max(0.0f, dot(normalize(inNormal), lightDir));
	shade = 0.2f + shade * 0.8f;

	gl_Position = vpMatrix * worldMatrix * BoneTransform * vec4(inPosition, 1.0);

}
