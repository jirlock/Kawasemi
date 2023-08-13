#version 410 core

in vec3 frag_pos;
in vec2 frag_uv;
in vec3 frag_norm;

uniform int use_texture;
uniform sampler2D mesh_texture;
uniform vec3 mesh_col;
uniform vec3 light_pos[POINTNUM];
uniform vec3 light_col[POINTNUM];
uniform float light_pow[POINTNUM];

out vec4 color;

vec3 calc_point(vec3 fragPos, vec3 fragNorm, float lightPow, vec3 lightPos, vec3 lightCol, vec3 diffuseCol)
{
    vec3 light2frag = fragPos - lightPos;
    float dist = length(light2frag);

    float cosTheta = clamp(dot(normalize(-1*light2frag), fragNorm), 0, 1);

    return cosTheta * lightPow * lightCol * diffuseCol / pow(dist, 2);
}

void main()
{
    color = vec4(0.0, 0.0, 0.0, 1.0);

    vec3 diffuse_col = vec3(0.0, 0.0, 0.0);
    if (use_texture == 1){
        diffuse_col = texture(mesh_texture, frag_uv).rgb;
    } else {
        diffuse_col = mesh_col;
    }

    for (int i=0; i<POINTNUM; i++){
        color.rgb += calc_point(frag_pos, frag_norm, light_pow[i], light_pos[i], light_col[i], diffuse_col);
    }
}