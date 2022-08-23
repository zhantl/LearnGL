#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "../Shader.hpp"
#include "../buffers/IndexBuffer.hpp"

struct VertexData
{
    glm::vec3 vertex;
    glm::vec2 uv;
    glm::vec4 color;
};

enum BufferSize
{
    SHADER_VERTEX_INDEX = 0,
    SHADER_UV_INDEX = 1,
    SHADER_COLOR_INDEX = 2,
    RENDER_MAX_SPRITES = 100,
    RENDER_VERTEX_SIZE = sizeof(VertexData),
    RENDER_SPRITE_SIZE = RENDER_VERTEX_SIZE * 4,
    RENDER_BUFFER_SIZE = RENDER_SPRITE_SIZE * RENDER_MAX_SPRITES,
    RENDER_INDICES_SIZE = RENDER_MAX_SPRITES * 6
};

class Node
{
public:
    Node();
    virtual ~Node();
    virtual void draw(Shader& shader);

    void addChild(Node *child);
    void setParent(Node *parent);
    void removeChildByName(const std::string &name);
    void removeChildren();
    void removeFormParent();

    const bool &isVisible() const { return m_visible; }
    const float &geScale() const { return m_scale; }
    const std::string &getName() const { return m_name; }
    const glm::vec3 &getPositon() const { return m_pos; }
    const glm::vec2 &getSize() const { return m_size; }
    const glm::vec4 &getColor() const { return m_color; }
    const std::vector<Node *> &getChildren() const { return m_children; }
    const Node *getParent() const { return m_parent; }

    void setVisible(bool &visible) { m_visible = visible; }
    void setName(const std::string &name) { m_name = name; }
    void setSize(const glm::vec2 &size) { m_size = size; }
    void setColor(const glm::vec4 &color) { m_color = color; }
    void setDirty(bool dirty);
    void setPosition(const glm::vec3 &pos);
    void setScale(float scale);
    void setRotation(float angle, glm::vec3 axis = glm::vec3(0., 0., 1.));

    const glm::mat4 &getTransform();
    virtual void genTransform();

protected:
    std::string m_name;
    float m_scale;
    float m_angle;
    glm::vec3 m_rotateAxis;
    bool m_visible;
    bool m_dirty;
    glm::vec3 m_pos;
    glm::vec2 m_size;
    glm::vec4 m_color;
    glm::mat4 m_matTransform;
    std::vector<Node *> m_children;
    Node *m_parent;
};