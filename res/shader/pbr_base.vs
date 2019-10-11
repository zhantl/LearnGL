#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
layout(location = 2) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view = mat4(1.f);
uniform mat4 model = mat4(1.f);

out Data
{
	vec3 v_pos;
	vec2 v_uv;
	vec3 v_normal;
} vs_out;

void main()
{
	vs_out.v_normal = mat3(model) * aNormal;
	vs_out.v_pos = vec3(model * vec4(aPos, 1.f));
	vs_out.v_uv = aUv;
	gl_Position = projection * view * vec4(vs_out.v_pos, 1.0) ;
}