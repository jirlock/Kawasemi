#version 410 core

in vec3 pos;
in vec2 uv;
in vec3 norm;

uniform mat4 model;
uniform mat4 rotation;
uniform mat4 camera_vp;

out vec3 frag_pos;
out vec2 frag_uv;
out vec3 frag_norm;

void main()
{
    gl_Position = camera_vp * model * vec4(pos, 1);
    frag_pos = (model * vec4(pos, 1)).xyz;
    frag_uv = uv;
    frag_norm = (rotation * vec4(norm, 1)).xyz;
}