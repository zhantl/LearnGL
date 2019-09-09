#pragma once
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <string>

class Texture
{
public:
    ~Texture();
    static Texture *create(const std::string &img_path);

    const std::string getName() const { return m_fileName; }
    const GLuint getID() const { return m_ID; }
    const int getWidth() const { return width; }
    const int getHegiht() const { return height; }

private:
    void init(const std::string &img_path);
    void loadTexture();
    Texture();

    std::string m_fileName;
    GLuint m_ID;
    int width, height;
    GLenum format;
};