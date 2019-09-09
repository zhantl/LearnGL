#include "batchRender2d.hpp"

BatchRender2d::BatchRender2d()
{
    init();
}

BatchRender2d::~BatchRender2d()
{
    delete m_IBO;
    texture_font_delete(m_FTfont);
    texture_atlas_delete(m_FTatlas);
}

void BatchRender2d::init()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, RENDER_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
    glEnableVertexAttribArray(SHADER_UV_INDEX);
    glEnableVertexAttribArray(SHADER_COLOR_INDEX);
    glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDER_VERTEX_SIZE, 0);
    glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDER_VERTEX_SIZE, (const void *)(offsetof(struct VertexData, uv)));
    glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, RENDER_VERTEX_SIZE, (const void *)(offsetof(struct VertexData, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint indices[RENDER_INDICES_SIZE];
    for (GLuint i = 0, offset = 0; i < RENDER_INDICES_SIZE; i += 6, offset += 4)
    {
        indices[i] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
    }

    m_IBO = new IndexBUffer(indices, RENDER_INDICES_SIZE);
    m_idxCount = 0;

    m_FTatlas = ftgl::texture_atlas_new(512, 512, 1);
    m_FTfont = ftgl::texture_font_new_from_file(m_FTatlas, 48, "res/font/arial.ttf");
}

void BatchRender2d::begin()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_buffer = static_cast<VertexData *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
}

void BatchRender2d::drawString(const std::string &text, const glm::vec3 &pos, const glm::vec4 &color)
{
    using namespace ftgl;

    int r = color.r * 255.f;
    int g = color.g * 255.f;
    int b = color.b * 255.f;
    int a = color.a * 255.f;

    unsigned int co = a << 24 | b << 16 | g << 8 | r;

    float x = pos.x;
    float y = pos.y;
    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text.at(i);
        texture_glyph_t *glyph = texture_font_get_glyph(m_FTfont, &c);

        if (glyph != nullptr)
        {
            float x0 = x + glyph->offset_x;
            float y0 = y + glyph->offset_y - glyph->height;
            float x1 = x0 + glyph->width;
            float y1 = y0 + glyph->height;

            float s0 = glyph->s0;
            float s1 = glyph->s1;
            float t0 = glyph->t1;
            float t1 = glyph->t0;

            m_buffer->vertex = glm::vec3(x0, y0, 0.);
            m_buffer->uv = glm::vec2(s0, t0);
            m_buffer->color = color;
            m_buffer++;

            m_buffer->vertex = glm::vec3(x0, y1, 0.);
            m_buffer->uv = glm::vec2(s0, t1);
            m_buffer->color = color;
            m_buffer++;

            m_buffer->vertex = glm::vec3(x1, y1, 0.);
            m_buffer->uv = glm::vec2(s1, t1);
            m_buffer->color = color;
            m_buffer++;

            m_buffer->vertex = glm::vec3(x1, y0, 0.);
            m_buffer->uv = glm::vec2(s1, t0);
            m_buffer->color = color;
            m_buffer++;

            m_idxCount += 6;
            x += glyph->advance_x;
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_FTatlas->id);
    glBindTexture(GL_TEXTURE_2D, m_FTatlas->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_FTatlas->width, m_FTatlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, m_FTatlas->data);
}

void BatchRender2d::submit(const Node *sprite)
{
}

void BatchRender2d::end()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRender2d::flush()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_FTatlas->id);

    glBindVertexArray(m_VAO);
    m_IBO->bind();

    glDrawElements(GL_TRIANGLES, m_idxCount, GL_UNSIGNED_INT, 0);

    m_IBO->unbind();
    glBindVertexArray(0);

    m_idxCount = 0;
}