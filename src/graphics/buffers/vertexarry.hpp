#pragma once

#include <glad/glad.h>
#include <vector>

#include "buffer.hpp"

class VertextAarry
{
public:
    VertextAarry();
    ~VertextAarry();
    void addBuffer(Buffer *buffer, GLuint index);
    void bind() const;
    void unbind() const;

private:
    std::vector<Buffer *> m_buffers;
    GLuint m_arrayID;
};