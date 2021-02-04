#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D Textura;

void main()
{
	color =texture(Textura, texCoords);
	//color = mix(texture(wood_texture, texCoords), texture(batman_texture, texCoords), 0.5);
}

