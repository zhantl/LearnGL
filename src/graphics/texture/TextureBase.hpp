#pragma once
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <string>

class TextureBase
{
public:
    virtual ~TextureBase();

    virtual void configTexture(bool isFill = false) = 0;
    virtual void bindTexture() const = 0;
    virtual void fillTexture(const void *data, GLenum pos = GL_TEXTURE_2D);

    void setSize(int width, int height);
    void setFormat(GLenum format) { m_internalFormat = m_imageFormat = format; }
    void setAllWrap(GLenum wrap) { m_wrapS = m_wrapT = m_wrapR = wrap; }
    void setAllFilter(GLenum fiter) { m_filterMax = m_filterMin = fiter; }
    void setInternalFormat(GLenum format) { m_internalFormat = format; }
    void setImageFormat(GLenum format) { m_imageFormat = format; }
    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }
    void setWrapS(GLenum wrapS) { m_wrapS = wrapS; }
    void setWrapT(GLenum wrapT) { m_wrapT = wrapT; }
    void setWrapR(GLenum wrapR) { m_wrapR = wrapR; }
    void setFilterMin(GLenum fliterMin) { m_filterMin = fliterMin; }
    void setFilterMax(GLenum fliterMax) { m_filterMax = fliterMax; }
    void setLod(bool isLod) { m_lod = isLod; }
    void setTextureDataType(GLenum type) { m_textureDataType = type; }

    const int getID() const { return m_ID; }
    const int getWidth() const { return m_width; }
    const int getHegiht() const { return m_height; }

protected:
    TextureBase();

    GLuint m_ID;
    int m_width, m_height;
    GLenum m_internalFormat;
    GLenum m_imageFormat;
    GLenum m_wrapS;
    GLenum m_wrapT;
    GLenum m_wrapR;
    GLenum m_filterMin;
    GLenum m_filterMax;

    bool m_lod;
    GLenum m_textureDataType;
};