#version 330 core
in vec3 color;
in vec2 UV;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main()
{
	// mix(A, B, val) = A * (1-val) + B * val
	// fragColor = mix(texture(texture1, UV), texture(texture2, UV), 0.2) * vec4(color, 1.0);
	fragColor = texture(texture1, UV);
}