#version 330 core

in vec3 textureDir;

uniform samplerCube skybox;

void main()
{
	gl_FragColor = texture(skybox, textureDir);
}
