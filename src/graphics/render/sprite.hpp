#pragma once

#include "node.hpp"
#include "texture.hpp"

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

private:
    GLuint m_VAO;
    GLuint m_VBO;
    IndexBUffer *m_IBO;
    Texture *m_texture;
    VertexData m_vertexDatas[4];
};