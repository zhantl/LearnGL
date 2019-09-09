#pragma once

#include <cstddef>
#include "render2d.hpp"
#include <freetyp-gl/texture-atlas.h>
#include <freetyp-gl/texture-font.h>
#include <graphics/fileUtils.hpp>

class BatchRender2d: public Render2d
{
public:
    enum
    {
        SHADER_VERTEX_INDEX = 0,
        SHADER_UV_INDEX     = 1,
        SHADER_COLOR_INDEX  = 2,
        RENDER_MAX_SPRITES  = 10000,
        RENDER_VERTEX_SIZE  = sizeof(VertexData),
        RENDER_SPRITE_SIZE  = RENDER_VERTEX_SIZE * 4,
        RENDER_BUFFER_SIZE  = RENDER_SPRITE_SIZE * RENDER_MAX_SPRITES,
        RENDER_INDICES_SIZE = RENDER_MAX_SPRITES * 6
    };

    BatchRender2d();
    ~BatchRender2d();
    void begin();
    void submit(const Node *sprite) override;
    void end();
    void flush() override;

    void drawString(const std::string& text, const glm::vec3& pos, const glm::vec4& color);

private:
    void init();
    GLuint m_VAO, m_VBO;
    GLsizei m_idxCount;
    IndexBUffer* m_IBO;
    VertexData *m_buffer;

    ftgl::texture_atlas_t *m_FTatlas;
    ftgl::texture_font_t *m_FTfont;
};