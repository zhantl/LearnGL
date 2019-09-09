#pragma once

#include <glad/glad.h>

class Buffer
{
public:
    Buffer(GLfloat *data, GLsizei count, GLuint componentCount);
    ~Buffer();
    void bind() const;
    void unbind() const;
    const GLuint getComponetCount() { return m_componetCount; };

private:
    GLuint m_bufferID;
    GLuint m_componetCount;
};