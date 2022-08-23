#include "Sphere.hpp"

Sphere::Sphere()
    : m_VAO(0),
      m_VBO(0),
      m_IBO(nullptr),
      m_texture(nullptr)
{
}

Sphere::~Sphere()
{
    delete m_IBO;
    m_IBO = nullptr;
    delete m_texture;
    m_texture = nullptr;
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

Sphere *Sphere::create()
{
    auto sp = new Sphere;
    if (sp)
        sp->init();
    return sp;
}

Sphere *Sphere::create(const std::string &img_path)
{
    auto sp = new Sphere;
    if (sp)
    {
        sp->init();
        sp->setTexture(img_path);
    }
    return sp;
}

void Sphere::init()
{
    std::vector<GLfloat> data;
    std::vector<GLuint> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const GLfloat PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            GLfloat xSegment = (GLfloat)x / (GLfloat)X_SEGMENTS;
            GLfloat ySegment = (GLfloat)y / (GLfloat)Y_SEGMENTS;
            GLfloat xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            GLfloat yPos = std::cos(ySegment * PI);
            GLfloat zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            data.push_back(xPos);
            data.push_back(yPos);
            data.push_back(zPos);
            data.push_back(xSegment);
            data.push_back(ySegment);
            data.push_back(xPos);
            data.push_back(yPos);
            data.push_back(zPos);
        }
    }

    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (y % 2 == 0) // even rows: y == 0, y == 2; and so on
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *)(5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_IBO = new IndexBuffer(&indices[0], (GLsizei)indices.size());
}

void Sphere::setTexture(const std::string &img_path)
{
    m_texture = Texture2D::create(img_path);
    setSize(glm::vec2(m_texture->getWidth(), m_texture->getHegiht()));
}

void Sphere::setTexture(Texture2D *texture)
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

void Sphere::genTransform()
{
    m_matTransform = glm::mat4(1.0);
    m_matTransform = glm::translate(m_matTransform, m_pos);
    m_matTransform = glm::rotate(m_matTransform, glm::radians(m_angle), m_rotateAxis);
    m_matTransform = glm::scale(m_matTransform, glm::vec3(m_scale, m_scale, m_scale));

    if (m_parent != nullptr)
    {
        m_matTransform *= m_parent->getTransform();
    }
}

void Sphere::draw(Shader &shader)
{
    shader.use();
    shader.setMat4("model", getTransform());

    glBindVertexArray(m_VAO);
    m_IBO->bind();

    if (m_texture)
    {
        glActiveTexture(GL_TEXTURE0);
        m_texture->bindTexture();
    }
    glDrawElements(GL_TRIANGLE_STRIP, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr);

    m_IBO->unbind();
    glBindVertexArray(0);
}