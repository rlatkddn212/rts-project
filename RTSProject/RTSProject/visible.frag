#version 420
in vec2 outtc;
out vec4 outColor;
uniform sampler2D uTexture;

void main()
{
    outColor = texture(uTexture, outtc);
}
