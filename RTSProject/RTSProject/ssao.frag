#version 430 core
out float FragColor;

in vec2 fragTexCoord;

layout(binding = 0) uniform sampler2D uGWorldPos;
layout(binding = 1) uniform sampler2D uGNormal;
layout(binding = 2) uniform sampler2D uGNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

const vec2 noiseScale = vec2(1024.0/4.0, 768.0/4.0); 

uniform mat4 projection;
uniform mat4 uViewMat;

void main()
{
    vec3 fragPos = (uViewMat * texture2D(uGWorldPos, fragTexCoord)).xyz;
    vec3 normal = normalize(texture2D(uGNormal, fragTexCoord).rgb);
    vec3 randomVec = normalize(texture2D(uGNoise, fragTexCoord * noiseScale).xyz);

	// �׶� ����Ʈ ���� ����ȭ
	// normal, tangent, bitangent 3���� ���͸� �����ϰ� ����
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
	// normal�� �߽������� �Ѵ�.
    mat3 TBN = mat3(tangent, bitangent, normal);
    
	// occlusion�� ���� ��Ų��.
	float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i)
    {
		// ������ ���� ��ġ
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius; 
        
		// �ؽ��� ��ǥ�� ��� ���� ��ũ�� ��ǥ�� ����
        vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
		// depth�� ��´�.
        float sampleDepth = (uViewMat * texture(uGWorldPos, offset.xy)).z;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}