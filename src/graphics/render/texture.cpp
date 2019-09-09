#include <iostream>
#include "texture.hpp"

Texture::Texture()
    : m_fileName(""), m_ID(-1)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

Texture* Texture::create(const std::string& img_path)
{
    auto self = new Texture();
    if(self)
        self->init(img_path);
    return self;
}

void Texture::init(const std::string& img_path)
{
    m_fileName = img_path;
    loadTexture();
}

void Texture::loadTexture()
{
    glGenTextures(1, &m_ID);
    int channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(m_fileName.c_str(), &width, &height, &channel, 0);

    if (data)
    {
        if (channel == 1)
            format = GL_RED;
        else if (channel == 3)
            format = GL_RGB;
        else if (channel == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at path: " << m_fileName << std::endl;
    stbi_image_free(data);
}