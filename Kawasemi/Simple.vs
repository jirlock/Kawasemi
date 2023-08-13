#version 410 core

in vec3 pos;

uniform mat4 model;
uniform mat4 camera_vp;


void main()
{
	gl_Position = camera_vp * model * vec4(pos, 1);
}
