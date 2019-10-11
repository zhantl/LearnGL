#include "framebuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(GLuint width, GLuint height)
    : m_fbo(0),
      m_depthRbo(0),
      m_clearType(0),
      m_width(width),
      m_height(height),
      m_colorTex(nullptr),
      m_depthTex(nullptr)
{
    glGenFramebuffers(1, &m_fbo);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
    if (m_depthRbo != 0)
    {
        glDeleteRenderbuffers(1, &m_depthRbo);
    }
    delete m_colorTex;
    m_colorTex = nullptr;
    delete m_depthTex;
    m_depthTex = nullptr;
}

FrameBuffer *FrameBuffer::createFrameBuffer(GLuint width, GLuint height)
{
    FrameBuffer *fb = new FrameBuffer(width, height);
    return fb;
}

void FrameBuffer::bindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBuffer::unBindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::attachColor(TextureBase *texture, GLenum texTarget, GLenum attachment, GLint lod)
{
    bindFrameBuffer();
    m_colorTex = texture;
    m_clearType |= GL_COLOR_BUFFER_BIT;
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, texture->getID(), lod);
    checkComplete();
    unBindFrameBuffer();
}

void FrameBuffer::attachDepthTex(TextureBase *texture, Attach type)
{
    bindFrameBuffer();
    m_depthTex = texture;
    m_clearType |= GL_DEPTH_BUFFER_BIT;
    if (type == Attach::DEPTHTEX)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getID(), 0);
    }
    else if (type == Attach::DEPTHCUBETEX)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getID(), 0);
    }
    checkComplete();
    unBindFrameBuffer();
}

void FrameBuffer::attachDepthRbo(Attach type)
{
    bindFrameBuffer();
    bool hasStencil = type == Attach::DEPTH24STENCIL8;
    m_clearType |= GL_DEPTH_BUFFER_BIT;
    if (hasStencil)
    {
        m_clearType |= GL_STENCIL_BUFFER_BIT;
    }
    glGenRenderbuffers(1, &m_depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, hasStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT24, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, hasStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    checkComplete();
    unBindFrameBuffer();
}

void FrameBuffer::checkComplete()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        std::cout << "FrameBuffer is complete!" << std::endl;
    else
        std::cout << "FrameBuffer is not complete!" << std::endl;
}

void FrameBuffer::beginRender()
{
    bindFrameBuffer();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(m_clearType);
}

void FrameBuffer::endRender()
{
    unBindFrameBuffer();
    auto window = Window::getInstance();
    glViewport(0, 0, window->getWidth(), window->getHeight());
}

void FrameBuffer::resetSize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;
    if (m_depthRbo != 0)
    {
        bindFrameBuffer();
        bool hasStencil = m_clearType & GL_STENCIL_BUFFER_BIT;
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, hasStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT24, m_width, m_height);
        unBindFrameBuffer();
    }
}