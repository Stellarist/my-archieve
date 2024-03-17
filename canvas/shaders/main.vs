#version 450 core

layout (location=0) in vec3 a_pos;
layout (location=1) in vec2 a_tex_coords;

out vec3 pos;
out vec2 tex_coords;

void main()
{
    gl_Position=vec4(a_pos, 1.0);
    pos=a_pos;
    tex_coords=vec2(a_tex_coords.x, 1-a_tex_coords.y);
}