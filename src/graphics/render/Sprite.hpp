#pragma once

#include "Node.hpp"
#include "../texture/Texture2D.hpp"

class Sprite: public Node
{
public:
    Sprite();
    ~Sprite();
    static Sprite *create();
    static Sprite *create(const std::string &img_path);
    void init();
    void draw(Shader& shader) override;
    void setTexture(const std::string &img_path);
    void setTexture(Texture2D *texture);

private:
    GLuint m_VAO;
    GLuint m_VBO;
    IndexBuffer *m_IBO;
    Texture2D *m_texture;
    VertexData m_vertexDatas[4];
};