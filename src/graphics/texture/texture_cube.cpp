#include "texture_cube.hpp"
#include <iostream>

TextureCube *TextureCube::create()
{
    auto self = new TextureCube();
    return self;
}

TextureCube *TextureCube::create(const std::vector<std::string> &imgs)
{
    auto self = new TextureCube();
    if (self)
        self->loadTextureData(imgs);
    return self;
}

void TextureCube::configTexture(bool isFill)
{
    bindTexture();
    if (isFill)
    {
        for (unsigned int i = 0; i < 6; ++i)
        {
            fillTexture(nullptr, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrapR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filterMax);
    if (m_lod)
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCube::fillTexture(const void *data, GLenum pos /*= GL_TEXTURE_2D*/)
{
    glTexImage2D(pos, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, m_textureDataType, data);
}

void TextureCube::bindTexture() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void TextureCube::loadTextureData(const std::vector<std::string> &imgs)
{
    configTexture();
    stbi_set_flip_vertically_on_load(false);
    int width = 0, height = 0, channel = 0;
    for (size_t i = 0; i < imgs.size(); i++)
    {
        void *data = nullptr;
        if (m_textureDataType == GL_FLOAT)
        {
            data = stbi_loadf(imgs[i].c_str(), &width, &height, &channel, 0);
        }
        else if (m_textureDataType == GL_UNSIGNED_BYTE)
        {
            data = stbi_load(imgs[i].c_str(), &width, &height, &channel, 0);
        }
        //如果没有设置宽、高就以第一张图的宽、高为准
        if (m_width == 0 || m_height == 0)
        {
            m_width = width;
            m_height = height;
        }
        if (data)
        {
            fillTexture(data, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
        else
            std::cout << "TextureCube failed to load at path: " << imgs[i].c_str() << std::endl;
        stbi_image_free(data);
    }
}