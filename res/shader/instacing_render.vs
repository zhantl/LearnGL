#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
layout(location = 5) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aTexCoord;
	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}