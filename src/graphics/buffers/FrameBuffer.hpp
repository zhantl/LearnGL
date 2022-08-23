#pragma once

#include <glad/glad.h>
#include "../Window.hpp"
#include "../texture/TextureBase.hpp"

class FrameBuffer
{
public:
    enum class Attach
    {
        NONECOLOR = 0,
        COLOR = 1,
        DEPTH24 = 2,
        DEPTH24STENCIL8 = 3,
        DEPTHTEX = 4,
        DEPTHCUBETEX = 5
    };
    static FrameBuffer *createFrameBuffer(GLuint width, GLuint height);
    ~FrameBuffer();

    void bindFrameBuffer();
    void unBindFrameBuffer();

    void attachColor(TextureBase *texture, GLenum texTarget = GL_TEXTURE_2D, GLenum attachment = GL_COLOR_ATTACHMENT0, GLint lod = 0);
    void attachDepthTex(TextureBase *texture, Attach type);
    void attachDepthRbo(Attach type);
    void checkComplete();

    void beginRender();
    void endRender();

    void resetSize(GLuint width, GLuint height);
    TextureBase *getColorTexture() const { return m_colorTex; }
    TextureBase *getDepthTexture() const { return m_depthTex; }

private:
    FrameBuffer(GLuint width, GLuint height);
    FrameBuffer(const FrameBuffer &) = delete;
    FrameBuffer &operator=(const FrameBuffer &) = delete;

protected:
    GLuint m_fbo;
    GLuint m_depthRbo;
    GLuint m_clearType;
    GLuint m_width, m_height;
    TextureBase *m_colorTex;
    TextureBase *m_depthTex;
};