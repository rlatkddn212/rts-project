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

	// 그람 슈미트 단위 직교화
	// normal, tangent, bitangent 3개의 벡터를 직교하게 만듬
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
	// normal을 중심축으로 한다.
    mat3 TBN = mat3(tangent, bitangent, normal);
    
	// occlusion을 누적 시킨다.
	float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i)
    {
		// 샘플의 월드 위치
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius; 
        
		// 텍스쳐 좌표를 얻기 위해 스크린 좌표로 투영
        vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
		// depth를 얻는다.
        float sampleDepth = (uViewMat * texture(uGWorldPos, offset.xy)).z;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}