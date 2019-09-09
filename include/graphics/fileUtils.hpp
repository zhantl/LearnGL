#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "stb_image/stb_image.h"
#include "glad/glad.h"

class FileUtils
{
public:
    static FileUtils *getInstance();
    static std::string readFile(const char *filepath);
    static unsigned int loadTexture(const char *path, bool gamga = false);
private:
    static FileUtils *fileUtils;
};
