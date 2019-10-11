#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class RenderToCube
{
public:
    static RenderToCube *create(const glm::vec3 &viewPos = glm::vec3(0., 0., 0.), GLfloat near = 0.1f, GLfloat far = 10.f);
    ~RenderToCube();
    void initMats(const glm::vec3 &viewPos);

private:
    RenderToCube(GLfloat near, GLfloat far);
    GLfloat m_near, m_far;

public:
    glm::mat4 m_projection;
    std::vector<glm::mat4> m_viewMats;
};