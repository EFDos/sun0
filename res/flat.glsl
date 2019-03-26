#VERTEX

#version 130
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 att_pos;
layout(location = 1) in vec4 att_color;

out vec4 color;

uniform mat4 model = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 viewport = mat4(1.0);

void main()
{
    gl_Position = projection * viewport * model * vec4(att_pos, 0.0, 1.0);
    color = att_color;
}

#FRAGMENT

#version 130

in vec4 color;

out vec4 frag_color;

void main()
{
    frag_color = color;
}
