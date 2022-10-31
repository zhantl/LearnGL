#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image/stb_image.h>
#include <glad/glad.h>

class FileUtils
{
public:
    static FileUtils *getInstance();
    static void destroy();
    std::string readFile(const std::string& filepath);
    std::string readFile(const char *filepath);
	unsigned int loadTexture(const std::string& path, bool flipy = false);
    unsigned int loadTexture(const char *path, bool flipy = false);
private:
    FileUtils() = default;
    ~FileUtils() = default;
    FileUtils(const FileUtils &) = delete;
    FileUtils &operator=(const FileUtils &) = delete;
    static FileUtils *fileUtils;
};
