#pragma once

#include <glad/glad.h>
#include "../render/texture2d.hpp"


class FrameBuffer
{
public:
    enum class FB_Type {
        NORMAL = 0,
        DEPTH = 1,
     };

    FrameBuffer(GLuint width, GLuint height, FB_Type type = FB_Type::NORMAL);
    ~FrameBuffer();
    static FrameBuffer *create(GLuint width, GLuint height, FB_Type type);
    void beginRender();
    void endRender();
    Texture2D *getTexture() { return m_texture; }

private:
    void generateFrameBuffer();
    FrameBuffer(const FrameBuffer &) = delete;
    FrameBuffer &operator=(const FrameBuffer &) = delete;

private:
    FB_Type m_type;
    GLuint m_fbo;
    GLuint m_width, m_height;

    Texture2D *m_texture;
    GLuint m_rbo;
};