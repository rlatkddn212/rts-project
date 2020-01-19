#version 420 core

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

uniform vec3 CameraPos;

uniform vec3 Direction;
uniform vec3 AmbientLight;
uniform vec3 DiffuseColor;
uniform vec3 SpecColor;

// N : Vertex Normal
// L : Light Dir
// V : View Dir
// R : reflect
vec3 CalcLight(vec3 N, vec3 L, vec3 V, vec3 R)
{
	vec3 ambientColour = AmbientLight;
	
	float diffuseFactor = max(dot(normalize(N), normalize(L)), 0.0f);
	vec3 diffuseColour =  DiffuseColor * diffuseFactor;
	
	vec3 specularColour = vec3(0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		float specularFactor = dot(V, R);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, 2.0);
			specularColour = SpecColor * specularFactor;
		}
	}

	return (ambientColour + diffuseColour + specularColour);
}

void main()
{
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
	//outColor = vec4(gbufferNorm,1.0);
	
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(-Direction);
	vec3 V = normalize(CameraPos - gbufferWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = CalcLight(N, L, V, R);

	outColor = vec4(gbufferDiffuse * Phong, 1.0);
	
}