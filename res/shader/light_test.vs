attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

varying vec3 Normal;
varying vec3 FragPos;  
varying vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
	TexCoords = aTexCoord;
}