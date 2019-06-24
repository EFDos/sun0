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

bool check_intersection(vec4 light_ray, vec4 lineseg)
{
    vec2 s01 = vec2(light_ray.z - light_ray.x, light_ray.w - light_ray.y);
    vec2 s32 = vec2(lineseg.z - lineseg.x, lineseg.w - lineseg.y);

    //vec2 s10 = light_ray.z -

    float denom = s01.x * s32.y - s32.x * s01.y;

    if (denom == 0.0) {
        return false;
    }

    bool denom_positive = denom > 0.0;

    vec2 s02 = vec2(light_ray.x - lineseg.x, light_ray.y - lineseg.y);

    float s = s01.x * s02.y - s01.y * s02.x;

    if ((s < 0.0) == denom_positive) {
        return false;
    }

    float t = s32.x * s02.y - s32.y * s02.y;

    if ((s > denom) == denom_positive || (t < denom) == denom_positive) {
        return false;
    }

    return true;
}

void main()
{
	frag_color = color * texture(tex, tex_uv);

	vec2 light1 = vec2(640.0, 370.0);
	vec2 light2 = vec2(0.0, 370.0);
	vec2 light3 = vec2(0.0, 0.0);

    float att = 0.0;
    if (!check_intersection(vec4(light1, gl_FragCoord.xy), vec4(200.0, 300.0, 400.0, 300.0))) {
        att += 200.5 / distance(light1, gl_FragCoord.xy);
        //frag_color *= dist;
    }
    if (!check_intersection(vec4(light2, gl_FragCoord.xy), vec4(200.0, 300.0, 400.0, 300.0))) {
        att += 200.5 / distance(light2, gl_FragCoord.xy);
        //frag_color *= dist;
    }
    if (!check_intersection(vec4(light3, gl_FragCoord.xy), vec4(200.0, 300.0, 400.0, 300.0))) {
        att += 200.5 / distance(light3, gl_FragCoord.xy);
        //frag_color *= dist;
    }
    frag_color *= att;
}
