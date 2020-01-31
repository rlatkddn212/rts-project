#version 420 core

in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

// G버퍼 텍스쳐
layout(binding = 0) uniform sampler2D uGDiffuse;
layout(binding = 1) uniform sampler2D uGNormal;
layout(binding = 2) uniform sampler2D uGWorldPos;
layout(binding = 3) uniform sampler2D uGSsao;
layout(binding = 4) uniform sampler2D uGShadowMap;

// 카메라 위치
uniform vec3 CameraPos;

// 빛의 방향
uniform vec3 Direction;
// 빛의 색상
uniform vec3 AmbientLight;
uniform vec3 DiffuseColor;
uniform vec3 SpecColor;

// 변환 메트릭
uniform mat4 worldToLightViewMatrix;
uniform mat4 lightViewToProjectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projMatrix;

// 그림자가 없다면 0.0, 그림자가 있다면 1.0
float ReadShadowMap(vec3 eyeDir)
{
	// 월드 좌표로 변환
	//mat4 cameraViewToWorldMatrix = inverse(cameraMatrix) * inverse(projMatrix);
	//vec4 worldPos = cameraViewToWorldMatrix * vec4(eyeDir, 1.0);
	//worldPos = worldPos / worldPos.w;

	// light 좌표로 변환
	mat4 cameraViewToProjectedLightSpace = lightViewToProjectionMatrix * worldToLightViewMatrix;
	vec4 lightPos = cameraViewToProjectedLightSpace * vec4(eyeDir.xyz, 1.0);
	lightPos = lightPos / lightPos.w;

	// [0.0~1.0] 사이 좌표로 변환
	lightPos = lightPos * 0.5 + 0.5;

	const float bias = 0.001;
	/*
	float depthValue = texture2D(uGShadowMap, lightPos.xy).r + bias;
	
	return (lightPos.z > depthValue) ? 1.0 : 0.0;
	*/

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(uGShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture2D(uGShadowMap, lightPos.xy + vec2(x, y) * texelSize).r; 
			// shadow map에 depth보다 멀리있다면 그림자가 생김
			shadow += (lightPos.z > pcfDepth + bias) ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	return shadow;
}

// N : Vertex Normal
// L : Light Dir
// V : View Dir
// R : reflect
vec3 CalcLight(float AmbientOcclusion, vec3 P, vec3 N, vec3 L, vec3 V, vec3 R)
{
	vec3 ambientColour = AmbientLight * AmbientOcclusion;
	
	float diffuseFactor = max(dot(normalize(N), normalize(L)), 0.0f);
	vec3 diffuseColour =  DiffuseColor * diffuseFactor;
	
	vec3 specularColour = vec3(0, 0, 0);
	
	if (diffuseFactor > 0.0f)
	{
		float specularFactor = dot(V, R);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, 2.0);
			specularColour = SpecColor * specularFactor;
		}
	}

	float shadow = ReadShadowMap(P);
	return (ambientColour + (diffuseColour + specularColour) *  (1.0 - shadow));
}

void main()
{
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
	float AmbientOcclusion = texture(uGSsao, fragTexCoord).r;

	//outColor = vec4(gbufferNorm,1.0);
	
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(-Direction);
	vec3 V = normalize(CameraPos - gbufferWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = CalcLight(AmbientOcclusion, gbufferWorldPos, N, L, V, R);
	float gamma = 2.2;
    
	//outColor = vec4(AmbientOcclusion);
	outColor = vec4(pow(gbufferDiffuse * Phong, vec3(1.0/gamma)) , 1.0);
}

