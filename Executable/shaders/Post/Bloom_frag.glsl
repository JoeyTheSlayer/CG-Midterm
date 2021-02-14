#version 420

layout(binding = 0) uniform sampler2D uTex;
uniform float u_Threshold;

out vec4 fragColor;

in vec2 TexCoords;

void main() 
{
	vec4 color = texture(uTex, TexCoords);

	float brightness = (color.r + color.g + color.b) / 3.0;

	if (brightness > u_Threshold)
	{
		fragColor = color;
	}
	else
	{
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}