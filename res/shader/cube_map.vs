#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
layout(location = 2) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view = mat4(1.f);
uniform mat4 model = mat4(1.f);

out vec3 w_pos;

void main()
{
	w_pos = aPos;
	gl_Position = projection * view * vec4(aPos, 1.f);
}