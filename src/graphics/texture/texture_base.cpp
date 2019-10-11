#include "texture_base.hpp"
#include <iostream>

TextureBase::TextureBase()
    : m_ID(0),
      m_width(0),
      m_height(0),
      m_internalFormat(GL_RGB),
      m_imageFormat(GL_RGB),
      m_wrapS(GL_CLAMP_TO_EDGE),
      m_wrapT(GL_CLAMP_TO_EDGE),
      m_wrapR(GL_CLAMP_TO_EDGE),
      m_filterMin(GL_LINEAR),
      m_filterMax(GL_LINEAR),
      m_lod(false),
      m_textureDataType(GL_UNSIGNED_BYTE)
{
    glGenTextures(1, &m_ID);
}

TextureBase::~TextureBase()
{
    glDeleteTextures(1, &m_ID);
}

void TextureBase::fillTexture(const void *data, GLenum pos /*= GL_TEXTURE_2D*/)
{
    glTexImage2D(pos, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, m_textureDataType, data);
}

void TextureBase::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}
