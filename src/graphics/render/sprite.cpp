#include "sprite.hpp"

Sprite::Sprite()
    : m_VAO(0),
      m_VBO(0),
      m_IBO(nullptr),
      m_texture(nullptr)
{
}

Sprite::~Sprite()
{
    removeFormParent();
    removeChildren();
    delete m_IBO;
    m_IBO = nullptr;
    delete m_texture;
    m_texture = nullptr;
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

Sprite *Sprite::create()
{
    auto sprite = new Sprite;
    if (sprite)
        sprite->init();
    return sprite;
}

Sprite *Sprite::create(const std::string &img_path)
{
    auto sprite = new Sprite;
    if (sprite)
    {
        sprite->init();
        sprite->setTexture(img_path);
    }
    return sprite;
}

void Sprite::init()
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

    GLuint indics[6] = {0, 1, 2, 2, 3, 0};
    m_IBO = new IndexBUffer(indics, 6);

    m_vertexDatas[0].uv = glm::vec2(0, 0);
    m_vertexDatas[1].uv = glm::vec2(0, 1);
    m_vertexDatas[2].uv = glm::vec2(1, 1);
    m_vertexDatas[3].uv = glm::vec2(1, 0);
}

void Sprite::setTexture(const std::string &img_path)
{
    m_texture = Texture2D::create(img_path);
    setSize(glm::vec2(m_texture->getWidth(), m_texture->getHegiht()));
}

void Sprite::setTexture(Texture2D *texture)
{
    if (m_texture == texture)
        return;
    if (m_texture)
    {
        delete m_texture;
        m_texture = nullptr;
    }
    m_texture = texture;
    setSize(glm::vec2(m_texture->getWidth(), m_texture->getHegiht()));
}

void Sprite::draw(Shader &shader)
{
    m_vertexDatas[0].vertex = glm::vec3(0., 0., 0.);
    m_vertexDatas[0].color = m_color;

    m_vertexDatas[1].vertex = glm::vec3(0., m_size.y, 0.);
    m_vertexDatas[1].color = m_color;

    m_vertexDatas[2].vertex = glm::vec3(m_size.x, m_size.y, 0.);
    m_vertexDatas[2].color = m_color;

    m_vertexDatas[3].vertex = glm::vec3(m_size.x, 0., 0.);
    m_vertexDatas[3].color = m_color;

    shader.use();
    shader.setMat4("model", getTransform());

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, BufferSize::RENDER_VERTEX_SIZE * 4, 0, GL_STATIC_DRAW);
    void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(data, m_vertexDatas, BufferSize::RENDER_VERTEX_SIZE * 4);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(m_VAO);
    m_IBO->bind();

    if (m_texture)
    {
        glActiveTexture(GL_TEXTURE0);
        m_texture->bindTexture();
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    m_IBO->unbind();
    glBindVertexArray(0);
}