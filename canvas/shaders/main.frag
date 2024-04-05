#version 450 core

in vec3 pos;
in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D texture0;

void main()
{
    frag_color=texture(texture0, tex_coords);
}
