#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform float opacity;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), opacity) * vec4(Color, 1.0);
}
