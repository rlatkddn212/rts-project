#version 420
in vec2 outtc;
out vec4 outColor;

void main()
{
    vec2 p = outtc - vec2(0.5,0.5);

	float d = length(p);
	float value = ((1.0 - d) * 1.3);
	
	if(value < 0.0)value = 0.0;
	if(value > 1.0)value = 1.0;

	outColor = vec4(value, value, value, value);
}
