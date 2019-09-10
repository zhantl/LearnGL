#include "label.hpp"

using namespace ftgl;

Label::Label()
    : m_VAO(0),
      m_VBO(0),
      m_IBO(nullptr),
      m_idxCount(0)
{
}

Label::~Label()
{
    removeFormParent();
    removeChildren();
    delete m_IBO;
    texture_font_delete(m_FTfont);
    texture_atlas_delete(m_FTatlas);
}

Label *Label::create()
{
    auto label = new Label;
    if (label)
        label->init();
    return label;
}

Label *Label::create(const std::string &text)
{
    auto label = new Label;
    if (label)
    {
        label->init();
        label->setString(text);
    }
    return label;
}

void Label::init()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glEnableVertexAttribArray(BufferSize::SHADER_VERTEX_INDEX);
    glEnableVertexAttribArray(BufferSize::SHADER_UV_INDEX);
    glEnableVertexAttribArray(BufferSize::SHADER_COLOR_INDEX);
    glVertexAttribPointer(BufferSize::SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, BufferSize::RENDER_VERTEX_SIZE, 0);
    glVertexAttribPointer(BufferSize::SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, BufferSize::RENDER_VERTEX_SIZE, (const void *)(offsetof(struct VertexData, uv)));
    glVertexAttribPointer(BufferSize::SHADER_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, BufferSize::RENDER_VERTEX_SIZE, (const void *)(offsetof(struct VertexData, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint indics[BufferSize::RENDER_INDICES_SIZE];
    for (size_t i = 0, idx = 0; i < BufferSize::RENDER_INDICES_SIZE; i += 6, idx += 4)
    {
        indics[i + 0] = idx + 0;
        indics[i + 1] = idx + 1;
        indics[i + 2] = idx + 2;
        indics[i + 3] = idx + 2;
        indics[i + 4] = idx + 3;
        indics[i + 5] = idx + 0;
    }
    m_IBO = new IndexBUffer(indics, BufferSize::RENDER_INDICES_SIZE);

    m_FTatlas = texture_atlas_new(512, 512, 1);
    m_FTfont = texture_font_new_from_file(m_FTatlas, 48, "res/font/fontthin.ttf");

    glGenTextures(1, &m_FTatlas->id);
    glBindTexture(GL_TEXTURE_2D, m_FTatlas->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Label::splietUtf8String(const std::string &text, std::vector<std::string> &result)
{
    for (int i = 0; i < text.length();)
    {
        char chr = text[i];
        //chr是0xxx xxxx，即ascii码
        if ((chr & 0x80) == 0)
        {
            result.push_back(text.substr(i, 1));
            ++i;
        } //chr是1111 1xxx
        else if ((chr & 0xF8) == 0xF8)
        {
            result.push_back(text.substr(i, 5));
            i += 5;
        } //chr是1111 xxxx
        else if ((chr & 0xF0) == 0xF0)
        {
            result.push_back(text.substr(i, 4));
            i += 4;
        } //chr是111x xxxx
        else if ((chr & 0xE0) == 0xE0)
        {
            result.push_back(text.substr(i, 3));
            i += 3;
        } //chr是11xx xxxx
        else if ((chr & 0xC0) == 0xC0)
        {
            result.push_back(text.substr(i, 2));
            i += 2;
        }
    }
}

void Label::setString(const std::string &text)
{
    m_idxCount = 0;
    m_text = text;

    std::vector<std::string> result;
    splietUtf8String(m_text, result);

    float x = 0.;
    float y = 0.;
    for (size_t i = 0; i < result.size(); i++)
    {
        texture_glyph_t *glyph = texture_font_get_glyph(m_FTfont, result[i].c_str());

        if (glyph != nullptr)
        {
            float x0 = x + glyph->offset_x;
            float y0 = 0. + glyph->offset_y - glyph->height;
            float x1 = x0 + glyph->width;
            float y1 = y0 + glyph->height;
            if (y < glyph->height)
            {
                y = glyph->height;
            }

            float s0 = glyph->s0;
            float s1 = glyph->s1;
            float t0 = glyph->t1;
            float t1 = glyph->t0;

            m_vertexDatas[0 + i * 4].vertex = glm::vec3(x0, y0, 0.);
            m_vertexDatas[0 + i * 4].uv = glm::vec2(s0, t0);
            m_vertexDatas[0 + i * 4].color = m_color;

            m_vertexDatas[1 + i * 4].vertex = glm::vec3(x0, y1, 0.);
            m_vertexDatas[1 + i * 4].uv = glm::vec2(s0, t1);
            m_vertexDatas[1 + i * 4].color = m_color;

            m_vertexDatas[2 + i * 4].vertex = glm::vec3(x1, y1, 0.);
            m_vertexDatas[2 + i * 4].uv = glm::vec2(s1, t1);
            m_vertexDatas[2 + i * 4].color = m_color;

            m_vertexDatas[3 + i * 4].vertex = glm::vec3(x1, y0, 0.);
            m_vertexDatas[3 + i * 4].uv = glm::vec2(s1, t0);
            m_vertexDatas[3 + i * 4].color = m_color;

            m_idxCount += 6;
            x += glyph->advance_x;
        }
    }

    m_size.x = x;
    m_size.y = y;
    m_dirty = true;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, m_FTatlas->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_FTatlas->width, m_FTatlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, m_FTatlas->data);
}

void Label::draw(Shader &shader)
{
    shader.use();
    shader.setMat4("model", getTransform());

    // m_vertexDatas[0].vertex = glm::vec3(100, 100, 0.);
    // m_vertexDatas[0].uv = glm::vec2(0, 1);

    // m_vertexDatas[1].vertex = glm::vec3(100, 300, 0.);
    // m_vertexDatas[1].uv = glm::vec2(0, 0);

    // m_vertexDatas[2].vertex = glm::vec3(500, 300, 0.);
    // m_vertexDatas[2].uv = glm::vec2(1, 0);

    // m_vertexDatas[3].vertex = glm::vec3(500, 100, 0.);
    // m_vertexDatas[3].uv = glm::vec2(1, 1);
    // m_idxCount = 6;

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    auto buff_size = BufferSize::RENDER_VERTEX_SIZE * 4 * m_idxCount / 6;
    glBufferData(GL_ARRAY_BUFFER, buff_size, 0, GL_STATIC_DRAW);
    void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(data, m_vertexDatas, buff_size);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(m_VAO);
    m_IBO->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FTatlas->id);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, m_idxCount, GL_UNSIGNED_INT, nullptr);

    m_IBO->unbind();
    glBindVertexArray(0);
}