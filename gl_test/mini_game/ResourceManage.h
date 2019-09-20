#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Shader.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <map>
#include <string>

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	static Shader		LoadShader(const std::string& vFile, const std::string& fFile, const std::string& gFile, const std::string& name);
	static Shader		GetShader(const std::string& name);
	static Texture2D	LoadTexture(const std::string& file, const std::string& name);
	static Texture2D	GetTexture(const std::string& name);
	static void			Clear();

private:
	ResourceManager(){}
	static Shader		loadShaderFromFile(const std::string& vFile, const std::string& fFile, const std::string& gFile = nullptr);
	static Texture2D	loadTextureFromFile(const std::string& file);
};

#endif //RESOURCE_MANAGER_H