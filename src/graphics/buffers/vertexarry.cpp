#include "vertexarry.hpp"


VertextAarry::VertextAarry()
{
    glGenVertexArrays(1, &m_arrayID);
}

VertextAarry::~VertextAarry()
{
    for (size_t i = 0; i < m_buffers.size(); i++)
    {
        delete m_buffers[i];
    }
    m_buffers.clear();
    glDeleteVertexArrays(1, &m_arrayID);
}

void VertextAarry::addBuffer(Buffer * buf, GLuint index)
{
    bind();
    buf->bind();

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, buf->getComponetCount(), GL_FLOAT, GL_FALSE, 0, nullptr);

    buf->unbind();
    unbind();
    m_buffers.push_back(buf);
}

void VertextAarry::bind() const
{
    glBindVertexArray(m_arrayID);
}

void VertextAarry::unbind() const
{
    glBindVertexArray(0);
}