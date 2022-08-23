#include "IndexBUffer.hpp"

IndexBUffer::IndexBUffer(GLuint *data, GLsizei count)
:m_count(count)
{
    glGenBuffers(1, &m_indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBUffer::~IndexBUffer()
{
    glDeleteBuffers(1, &m_indexBufferID);
}

void IndexBUffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
}

void IndexBUffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}