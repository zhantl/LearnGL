#include "texture_cube.hpp"
#include <iostream>

TextureCube::TextureCube()
    : m_ID(-1)
{
}

TextureCube::~TextureCube()
{
    glDeleteTextures(1, &m_ID);
}

TextureCube *TextureCube::create(const std::vector<std::string> &imgs)
{
    auto self = new TextureCube();
    if (self)
        self->loadCubeTexture(imgs);
    return self;
}

void TextureCube::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void TextureCube::loadCubeTexture(const std::vector<std::string> &imgs)
{
    stbi_set_flip_vertically_on_load(false);
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    int width, height, channel;
    GLenum format;
    for (size_t i = 0; i < imgs.size(); i++)
    {
        unsigned char *data = stbi_load(imgs[i].c_str(), &width, &height, &channel, 0);
        if (data)
        {
            if (channel == 1)
                format = GL_RED;
            else if (channel == 3)
                format = GL_RGB;
            else if (channel == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
            std::cout << "TextureCube failed to load at path: " << imgs[i].c_str() << std::endl;
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}