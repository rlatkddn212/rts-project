#version 420
in vec2 outtc;
out vec4 outColor;
uniform sampler2D uTexture;

void main()
{
     vec4 c = texture(uTexture, outtc);
	 
	 if (c.r > 0.1)
	 {
		outColor = vec4(0.3, 0.3, 0.3, 1.0);
	 }
	 else
	 {
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	 }
}
