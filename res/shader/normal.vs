#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
layout(location = 2) in vec4 aColor;

uniform mat4 proj;
uniform mat4 view = mat4(1.f);
uniform mat4 model = mat4(1.f);

out Data
{
	vec4 v_color;
	vec2 v_uv;
	vec4 v_pos;
} vs_out;

void main()
{
	vs_out.v_color = aColor;
	vs_out.v_pos = model * vec4(aPos, 1.f);
	vs_out.v_uv = aUv;
	gl_Position = proj * view * vs_out.v_pos;
}