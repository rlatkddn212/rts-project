#version 420 core

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

struct DirectionalLight
{
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
};

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLight;

void main()
{
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(-uDirLight.mDirection);
	vec3 V = normalize(uCameraPos - gbufferWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * dot(N, L);
	}

	Phong = clamp(Phong, 0.0, 1.0);

	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}