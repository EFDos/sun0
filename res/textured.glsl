#VERTEX

#version 130
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 att_pos;
layout(location = 1) in vec2 att_tex_uv;
layout(location = 2) in vec4 att_color;

out vec4 color;
out vec2 tex_uv;

uniform mat4 projection = mat4(1.0);
uniform mat4 viewport = mat4(1.0);

void main()
{
    gl_Position = projection * viewport * vec4(att_pos, 0.0, 1.0);
    color = att_color;
    tex_uv = att_tex_uv;
}

#FRAGMENT

#version 130

in vec4 color;
in vec2 tex_uv;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    frag_color = color * texture(tex, tex_uv);
}
