#include <iostream>
#include "Texture2D.hpp"

Texture2D::Texture2D()
    : TextureBase(), m_fileName("")
{
}

Texture2D *Texture2D::create()
{
    auto self = new Texture2D();
    return self;
}

Texture2D *Texture2D::create(const std::string &img_path)
{
    auto self = new Texture2D();
    if (self)
        self->loadTextureData(img_path);
    return self;
}

void Texture2D::configTexture(bool isFill)
{
    bindTexture();
    if (isFill)
    {
        fillTexture(nullptr);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);
    if (m_lod)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture2D::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::loadTextureData(const std::string &img_path)
{
    m_fileName = img_path;
    int width = 0, height = 0, channel = 0;
    stbi_set_flip_vertically_on_load(true);
    void *data = nullptr;
    if (m_textureDataType == GL_FLOAT)
    {
        data = stbi_loadf(img_path.c_str(), &width, &height, &channel, 0);
    }
    else if (m_textureDataType == GL_UNSIGNED_BYTE)
    {
        data = stbi_load(img_path.c_str(), &width, &height, &channel, 0);
    }
    if (m_width == 0 || m_height == 0)
    {
        m_width = width;
        m_height = height;
    }
    if (data)
    {
        if (channel == 1)
            m_imageFormat = GL_RED;
        else if (channel == 3)
            m_imageFormat = GL_RGB;
        else if (channel == 4)
            m_imageFormat = GL_RGBA;

        configTexture();
        fillTexture(data);
    }
    else
        std::cout << "Texture2D failed to load at path: " << m_fileName << std::endl;
    stbi_image_free(data);
}