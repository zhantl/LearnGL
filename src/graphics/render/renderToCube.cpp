#include "renderToCube.hpp"

RenderToCube::RenderToCube(GLfloat near, GLfloat far)
    : m_near(near),
      m_far(far)
{
}

RenderToCube::~RenderToCube()
{
}

RenderToCube *RenderToCube::create(const glm::vec3 &viewPos, GLfloat near, GLfloat far)
{
    auto rtc = new RenderToCube(near, far);
    if (rtc)
    {
        rtc->initMats(viewPos);
    }
    return rtc;
}

void RenderToCube::initMats(const glm::vec3 &viewPos)
{
    m_projection = glm::perspective(glm::radians(90.0f), 1.0f, m_near, m_far);
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    m_viewMats.push_back(glm::lookAt(viewPos, viewPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
}