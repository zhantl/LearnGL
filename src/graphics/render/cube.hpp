#pragma once

#include "node.hpp"
#include "../texture/texture_base.hpp"
#include "../texture/texture2d.hpp"

class Cube: public Node
{
public:
    Cube();
    ~Cube();
    static Cube *create();
    static Cube *create(const std::string &img_path);
    void init();
    void draw(Shader& shader) override;
    void setTexture(TextureBase *texture);
    void genTransform() override;

private:
    GLuint m_VAO;
    GLuint m_VBO;
    TextureBase *m_texture;
};