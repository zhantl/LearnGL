#pragma once

#include <string>
#include <freetyp-gl/texture-atlas.h>
#include <freetyp-gl/texture-font.h>

#include "node.hpp"
#include "../texture/texture2d.hpp"

class Label: public Node
{
public:
    Label();
    ~Label();
    static Label *create();
    static Label *create(const std::string &text);
    void init();
    void draw(Shader& shader) override;
    void setString(const std::string &text);

    Texture2D *getTexture() { return m_texture; } // for text

private:
    void splietUtf8String(const std::string& str, std::vector<std::string>& result);

private:
    GLuint m_VAO;
    GLuint m_VBO;
    IndexBUffer *m_IBO;
    GLuint m_idxCount;
    VertexData m_vertexDatas[BufferSize::RENDER_MAX_SPRITES];

    ftgl::texture_atlas_t *m_FTatlas;
    ftgl::texture_font_t *m_FTfont;

    std::string m_text;
    Texture2D *m_texture;
};
