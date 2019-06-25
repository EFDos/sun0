#VERTEX

#version 130
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 att_pos;
layout(location = 1) in vec2 att_tex_uv;
layout(location = 2) in vec4 att_color;

out vec4 color;
out vec2 tex_uv;

uniform mat4 model = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 viewport = mat4(1.0);

void main()
{
    gl_Position = projection * viewport * model * vec4(att_pos, 0.0, 1.0);
    color = att_color;
    tex_uv = att_tex_uv;
}

#FRAGMENT

#version 130

in vec4 color;
in vec2 tex_uv;

out vec4 frag_color;

uniform sampler2D tex;

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
bool check_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y/*, float *i_x, float *i_y*/)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        //if (i_x != NULL)
        //    *i_x = p0_x + (t * s1_x);
        //if (i_y != NULL)
        //    *i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

void main()
{
	frag_color = color * texture(tex, tex_uv);

	vec2 light1 = vec2(640.0, 370.0);
	vec2 light2 = vec2(0.0, 370.0);
	vec2 light3 = vec2(0.0, 0.0);

    float att = 1.0;
    if (!check_intersection(light1.x, light1.y, gl_FragCoord.x, gl_FragCoord.y, 200.0, 300.0, 400.0, 300.0)) {
        att += 100.5 / distance(light1, gl_FragCoord.xy);
    }
    if (!check_intersection(light2.x, light2.y, gl_FragCoord.x, gl_FragCoord.y, 200.0, 300.0, 400.0, 300.0)) {
        att += 200.5 / distance(light2, gl_FragCoord.xy);
    }
    if (!check_intersection(light3.x, light3.y, gl_FragCoord.x, gl_FragCoord.y, 200.0, 300.0, 400.0, 300.0)) {
        att += 200.5 / distance(light3, gl_FragCoord.xy);
    }
    frag_color *= att;
}
