#pragma once
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <string>

class Texture2D
{
public:
    ~Texture2D();
    static Texture2D *create();
    static Texture2D *create(const std::string &img_path);

    void generateTexture();
    void fillTexture(const unsigned char* data);
    void bind();

    void setFormat(GLuint format) { m_format = format; }
    void setWidth(GLuint width) { m_width = width; }
    void setHeight(GLuint height) { m_height = height; }
    void setWrapS(GLuint wrapS) { m_wrapS = wrapS; }
    void setWrapT(GLuint wrapT) { m_wrapT = wrapT; }
    void setFilterMin(GLuint fliterMin) { m_filterMin = fliterMin; }
    void setFilterMax(GLuint fliterMax) { m_filterMax = fliterMax; }
    
    const int getID() const { return m_ID; }
    const std::string getName() const { return m_fileName; }
    const int getWidth() const { return m_width; }
    const int getHegiht() const { return m_height; }

private:
    void loadTextureWithImage(const std::string &img_path);
    Texture2D();

    std::string m_fileName;
    GLuint m_ID;
    int m_width, m_height;
    GLenum m_format;
    GLuint m_wrapS;
	GLuint m_wrapT;
	GLuint m_filterMin;
	GLuint m_filterMax;
};