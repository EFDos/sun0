#VERTEX

#version 130
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 att_pos;
layout(location = 1) in vec2 att_tex_uv;
layout(location = 2) in vec4 att_color;

out vec4 color;
out vec2 tex_uv;
out mat4 view;

uniform mat4 projection = mat4(1.0);
uniform mat4 viewport = mat4(1.0);

void main()
{
    gl_Position = projection * vec4(att_pos, 0.0, 1.0);
    color = att_color;
    tex_uv = att_tex_uv;
    view = viewport;
    /*for (int i = 0 ; i < MAX_N ; ++i) {
        //vec4 light_transform = viewport * vec4(lights[i].pos, 0.0, 1.0);
        forward_lights[i].pos = vec2(light_transform.x, -light_transform.y);
        //forward_lights[i].intensity = lights[i].intensity;
        //forward_lights[i].color = lights[i].color;
    }*/
}

#FRAGMENT

#version 130
#define MAX_N 10

struct Light
{
    vec2 pos;
    float intensity;
    vec3 color;
};

in vec4 color;
in vec2 tex_uv;
in mat4 view;

out vec4 frag_color;

uniform Light lights[MAX_N] = Light[MAX_N](
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0)),
    Light(vec2(0.0), 0.0, vec3(1.0))
);

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

    float att = 0.0;
    vec3 col = vec3(0.0);
    int steps = 0;
    for (int i = 0 ; i < MAX_N ; ++i) {
        if (lights[i].intensity == 0.0) {
            continue;
        }

        vec4 light_transform = view * vec4(lights[i].pos, 0.0, 1.0);
        vec2 pos = vec2(light_transform.x, -light_transform.y);
        if (!check_intersection(pos.x, pos.y, gl_FragCoord.x, gl_FragCoord.y, 200.0, 300.0, 400.0, 300.0)) {
            att += lights[i].intensity / distance(pos, gl_FragCoord.xy);
            col += lights[i].color * att * 1.5;
        }
        ++steps;
    }
    col /= steps;
    frag_color *= vec4(col.xyz, 1.0);
}
