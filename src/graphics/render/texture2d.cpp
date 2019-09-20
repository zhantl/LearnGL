#include "texture2d.hpp"
#include <iostream>

Texture2D::Texture2D()
    : m_fileName(""),
      m_ID(0),
      m_width(0),
      m_height(0),
      m_format(GL_RGB),
      m_wrapS(GL_REPEAT),
      m_wrapT(GL_REPEAT),
      m_filterMin(GL_LINEAR),
      m_filterMax(GL_LINEAR)
{
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_ID);
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
        self->loadTextureWithImage(img_path);
    return self;
}

void Texture2D::generateTexture()
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::fillTexture(const unsigned char* data)
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::loadTextureWithImage(const std::string &img_path)
{
    m_fileName = img_path;
    int channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(m_fileName.c_str(), &m_width, &m_height, &channel, 0);

    if (data)
    {
        if (channel == 1)
            m_format = GL_RED;
        else if (channel == 3)
            m_format = GL_RGB;
        else if (channel == 4)
            m_format = GL_RGBA;

        generateTexture();
        fillTexture(data);
    }
    else
        std::cout << "Texture2D failed to load at path: " << m_fileName << std::endl;
    stbi_image_free(data);
}