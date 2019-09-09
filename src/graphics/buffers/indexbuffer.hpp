#pragma once

#include <glad/glad.h>

class IndexBUffer
{
public:
    IndexBUffer(GLuint *data, GLsizei count);
    ~IndexBUffer();
    void bind() const;
    void unbind() const;
    const GLsizei getCount() const { return m_count; };

private:
    GLuint m_indexBufferID;
    GLsizei m_count;
};