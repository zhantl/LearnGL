#pragma once

#include <glad/glad.h>

class IndexBuffer
{
public:
    IndexBuffer(GLuint *data, GLsizei count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    const GLsizei getCount() const { return m_count; };

private:
    GLuint m_IndexBufferID;
    GLsizei m_count;
};