#version 420 core
in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

struct PointLight
{
	vec3 mWorldPos;
	vec3 mDiffuseColor;
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;
uniform vec2 uScreenDimensions;

void main()
{
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);
		float intensity = smoothstep(uPointLight.mInnerRadius,
									 uPointLight.mOuterRadius, dist);
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
								vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL;
	}

	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
