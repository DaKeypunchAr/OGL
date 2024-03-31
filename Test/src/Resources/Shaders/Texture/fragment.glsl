#version 450 core

in vec2 texCoords;

uniform sampler2D tex0;
out vec4 oColor;

void main()
{
	oColor = texture(tex0, texCoords);
}