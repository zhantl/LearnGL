#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 textureDir;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	textureDir = aPos;
	mat4 rotView = mat4(mat3(view));
	gl_Position = projection * rotView * vec4(aPos, 1.0);
	gl_Position = gl_Position.xyww;
}