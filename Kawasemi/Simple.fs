#version 410 core

uniform vec3 frag_col;

out vec4 color;

void main()
{
	color.a = 1.0;
	color.rgb = frag_col;
}