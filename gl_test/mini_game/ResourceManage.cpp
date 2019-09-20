#include "ResourceManage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stb_image.h>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const std::string& vFile, const std::string& fFile, const std::string& gFile, const std::string& name)
{
	Shaders[name] = loadShaderFromFile(vFile, fFile, gFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const std::string& file, const std::string& name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders)
	{
		glDeleteProgram(iter.second.ID);
	}

	for (auto iter : Textures)
	{
		glDeleteTextures(1, &iter.second.ID);
	}
}

Shader ResourceManager::loadShaderFromFile(const std::string& vFile, const std::string& fFile, const std::string& gFile /* = nullptr */)
{
	std::string vCode;
	std::string fCode;
	std::string gCode;

	try
	{
		std::ifstream vShaderFile(vFile);
		std::ifstream fShaderFile(fFile);
		std::stringstream vStream, fStream;
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vCode = vStream.str();
		fCode = fStream.str();
		if (!gFile.empty())
		{
			std::ifstream gShaderFile(gFile);
			std::stringstream gStream;
			gStream << gShaderFile.rdbuf();
			gShaderFile.close();
			gCode = gStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::Shader: Failed to read Shader files" << std::endl;
	}

	Shader Shader;
	Shader.Compile(vCode.c_str(), fCode.c_str(), gCode.empty() ? "" : gCode.c_str());
	return Shader;
}

Texture2D ResourceManager::loadTextureFromFile(const std::string& file)
{
	Texture2D texture;

	int width, height, nrComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		texture.Internal_Format = format;
		texture.Image_Format = format;
		texture.Generate(width, height, data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << file << std::endl;
	}
	stbi_image_free(data);

	return texture;
}
