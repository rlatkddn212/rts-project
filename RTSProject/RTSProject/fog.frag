#version 420
in vec2 outtc;
out vec4 outColor;
layout(binding = 0) uniform sampler2D image01;
layout (binding = 1) uniform sampler2D image02;

void main()
{
    outColor = max(texture(image01, outtc), texture(image02, outtc));
}
