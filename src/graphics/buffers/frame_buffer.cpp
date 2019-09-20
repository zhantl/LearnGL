#include "frame_buffer.hpp"
#include <graphics/window.hpp>
#include <iostream>

FrameBuffer::FrameBuffer(GLuint width, GLuint height, FB_Type type)
    : m_type(type),
      m_fbo(0),
      m_width(width),
      m_height(height)
{
    glGenFramebuffers(1, &m_fbo);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
}

FrameBuffer *FrameBuffer::create(GLuint width, GLuint height, FB_Type type)
{
    auto fb = new FrameBuffer(width, height, type);
    if (fb)
    {
        fb->generateFrameBuffer();
    }
    return fb;
}

void FrameBuffer::generateFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    switch (m_type)
    {
    case FB_Type::NORMAL:
    {
        m_texture = Texture2D::create();
        m_texture->setWidth(m_width);
        m_texture->setHeight(m_height);
        m_texture->generateTexture();
        m_texture->fillTexture(nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->getID(), 0);
        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        break;
    }
    case FB_Type::DEPTH:
    {
        m_texture = Texture2D::create();
        m_texture->setWidth(m_width);
        m_texture->setHeight(m_height);
        m_texture->setWrapS(GL_CLAMP_TO_EDGE);
        m_texture->setWrapT(GL_CLAMP_TO_EDGE);
        m_texture->setFilterMax(GL_NEAREST);
        m_texture->setFilterMin(GL_NEAREST);
        m_texture->generateTexture();
        m_texture->fillTexture(nullptr);
        m_texture->bind();
        GLfloat borderColor[] = {1., 1., 1., 1.};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture->getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        break;
    }
    default:
        break;
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::beginRender()
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (m_type == FB_Type::NORMAL)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else if (m_type == FB_Type::DEPTH)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}

void FrameBuffer::endRender()
{
    auto window = Window::getInstance();
    glViewport(0, 0, window->getWidth(), window->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}