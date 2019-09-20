#pragma once
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <string>
#include <vector>

class TextureCube
{
public:
    ~TextureCube();
    static TextureCube *create(const std::vector<std::string> &imgs);
    void bind();

private:
    void loadCubeTexture(const std::vector<std::string> &imgs);
    TextureCube();

    GLuint m_ID;
};