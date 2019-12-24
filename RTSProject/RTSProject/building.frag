#version 430 core

layout(binding = 0) uniform sampler2D image01;
in vec2 fragTexCoord;
out vec4 color;
in float shade;

uniform vec3 buildingColor;

void main()
{
	vec4 c0 = texture(image01, fragTexCoord);
	float Inv = 1.0f - c0.a;
	
	vec3 c1 = vec3(c0.rgb * Inv + vec3(1.0, 0.0, 0.0) * c0.a);
	color = vec4(c1 * buildingColor * shade, 1.0f);
}