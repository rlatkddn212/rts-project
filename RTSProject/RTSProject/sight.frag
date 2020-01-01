#version 420
in vec2 outtc;
out vec4 outColor;

void main()
{
    vec2 p = outtc - vec2(0.5,0.5);

	// 중심에서 거리
	float d = length(p);
	// 거리가 가까우면 값이 커짐
	float value = ((0.5 - d) * 2);
	
	if(value < 0.0)value = 0.0;
	if(value > 1.0)value = 1.0;

	outColor = vec4(value, value, value, value);
}
