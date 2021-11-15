#version 410 core

in vec3 vert_normal_view;
in vec3 vert_color;

uniform float specular_coef;

out vec4 out_color;
out vec4 out_normal;

void main(){
    out_color.rgb = vert_color;
    out_color.a = specular_coef;
    out_normal.xyz = vert_normal_view;
    out_normal.w = 1;
}