#version 410 core

uniform mat3 normal_matrix;

in vec3 vtx_position;
in vec3 vtx_color;
in vec3 vtx_normal;

out vec3 vert_normal_view;
out vec3 vert_color;

void main()
{
    gl_Position = vec4(vtx_position,1.0);

    vert_normal_view = normalize(normal_matrix * vtx_normal);

    vert_color = vtx_color;
}
