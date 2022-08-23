#pragma once

#include "../Shader.hpp"
#include "../texture/TextureCube.hpp"
#include "../buffers/IndexBuffer.hpp"


class SkyBox
{
public:
    SkyBox();
    ~SkyBox();
    static SkyBox *create(TextureCube* cube_texture);
    static SkyBox *create(const std::vector<std::string> &imgs);
    void init();
    void draw();
    void setTexture(TextureCube* cube_texture);
    Shader *getShader() const { return m_shader; }

private:
    GLuint m_VAO;
    GLuint m_VBO;
    IndexBUffer *m_IBO;
    TextureCube *m_texture;
    Shader *m_shader;
};