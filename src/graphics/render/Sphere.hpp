#pragma once

#include "Node.hpp"
#include "../texture/Texture2D.hpp"

class Sphere : public Node
{
public:
    Sphere();
    ~Sphere();
    static Sphere *create();
    static Sphere *create(const std::string &img_path);
    void init();
    void draw(Shader &shader) override;
    void setTexture(const std::string &img_path);
    void setTexture(Texture2D *texture);
    void genTransform() override;

private:
    GLuint m_VAO;
    GLuint m_VBO;
    IndexBUffer *m_IBO;
    Texture2D *m_texture;
};