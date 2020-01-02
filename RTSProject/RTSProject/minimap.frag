#version 420
in vec2 outtc;
out vec4 outColor;
layout (binding = 0) uniform sampler2D mapTexture;
layout (binding = 1) uniform sampler2D fogTexture;

void main()
{
    outColor = texture(mapTexture, outtc) * texture(fogTexture, outtc);
}
