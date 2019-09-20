#include "sky_box.hpp"

SkyBox::SkyBox()
    : m_VAO(0),
      m_VBO(0),
      m_IBO(nullptr),
      m_texture(nullptr),
      m_shader(nullptr)
{
}

SkyBox::~SkyBox()
{
    delete m_IBO;
    m_IBO = nullptr;
    delete m_texture;
    m_texture = nullptr;
    delete m_shader;
    m_shader = nullptr;
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

SkyBox *SkyBox::create(TextureCube *cube_texture)
{
    auto box = new SkyBox;
    if (box)
    {
        box->init();
        box->setTexture(cube_texture);
    }

    return box;
}

SkyBox *SkyBox::create(const std::vector<std::string> &imgs)
{
    auto box = new SkyBox;
    if (box)
    {
        box->init();
        box->setTexture(TextureCube::create(imgs));
    }
    return box;
}

void SkyBox::init()
{
    GLfloat skyboxVertices[] = {
        //font
        -1.0f, -1.0f,  1.0f, //左下角 0
         1.0f, -1.0f,  1.0f, //右下角 1
         1.0f,  1.0f,  1.0f, //右上角 2
        -1.0f,  1.0f,  1.0f, //左上角 3

        //back
        -1.0f,  1.0f, -1.0f, //左上角 4
        -1.0f, -1.0f, -1.0f, //左下角 5
         1.0f, -1.0f, -1.0f, //右下角 6
         1.0f,  1.0f, -1.0f, //右上角 7
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint indics[] = {
        0, 1, 2,  2, 3, 0,
        0, 3, 4,  4, 5, 0,
        0, 1, 6,  6, 5, 0,
        1, 6, 7,  7, 2, 1,
        4, 3, 2,  2, 7, 4,
        4, 7, 6,  6, 5, 4,
    };
    m_IBO = new IndexBUffer(indics, 36);

    m_shader = new Shader("./res/shader/sky_box.vs", "./res/shader/sky_box.fs");
}

void SkyBox::setTexture(TextureCube *cube_texture)
{
    m_texture = cube_texture;
}

void SkyBox::draw()
{
    m_shader->use();
    glBindVertexArray(m_VAO);
    m_IBO->bind();

    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    m_IBO->unbind();
    glBindVertexArray(0);
}