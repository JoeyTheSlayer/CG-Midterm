#version 420

layout (binding = 0) uniform sampler2D uScene;
layout (binding = 1) uniform sampler2D uBloom;

in vec2 TexCoords;

out vec4 fragColor;

void main() 
{
	vec4 color_A = texture(uScene, TexCoords);
	vec4 color_B = texture(uBloom, TexCoords);

	fragColor = 1.0 - (1.0 - color_A) * (1.0 - color_B);
}