#include "FileUtils.hpp"

FileUtils* FileUtils::fileUtils = nullptr;

FileUtils* FileUtils::getInstance()
{
    if(!fileUtils)
        fileUtils = new FileUtils;
    return fileUtils;
}

void FileUtils::destroy()
{
	if(fileUtils)
	{
		delete fileUtils;
	}
	fileUtils = nullptr;
}

std::string FileUtils::readFile(const std::string& filepath)
{
	return this->readFile(filepath.c_str());
}

std::string FileUtils::readFile(const char *filepath)
{
    std::ifstream file;
	std::stringstream fstream;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(filepath);
        fstream << file.rdbuf();
        file.close();
    }
    catch (const std::ifstream::failure e)
    {
        std::cout << "ERROR read file fail " << filepath << std::endl;
    }
	return fstream.str();
}

unsigned int FileUtils::loadTexture(const std::string& path, bool gamma /* = false */)
{
	return this->loadTexture(path.c_str());
}

unsigned int FileUtils::loadTexture(const char *path, bool gamma /* = false */)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, channel;
	unsigned char* data = stbi_load(path, &width, &height, &channel, 0);

	if (data)
	{
		GLenum format;
		if (channel == 1)
			format = GL_RED;
		else if (channel == 3)
			format = GL_RGB;
		else if (channel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} 
	else
		std::cout << "Texture failed to load at path: " << path << std::endl;
	stbi_image_free(data);

	return textureID;
}