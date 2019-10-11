#include "node.hpp"

Node::Node()
    : m_scale(1.0),
      m_angle(0.0),
      m_rotateAxis(glm::vec3(0., 0., 1.)),
      m_visible(true),
      m_dirty(false),
      m_pos(glm::vec3(0., 0., 0.)),
      m_size(glm::vec2(0, 0.)),
      m_color(glm::vec4(1., 1., 1., 1.)),
      m_matTransform(glm::mat4(1.0)),
      m_parent(nullptr)
{
}

Node::~Node()
{
    removeFormParent();
    removeChildren();
}

void Node::addChild(Node *child)
{
    m_children.push_back(child);
    child->setParent(this);
}

void Node::setParent(Node *parent)
{
    m_parent = parent;
}

void Node::removeChildByName(const std::string &name)
{
    for (auto child = m_children.begin(); child != m_children.end(); ++child)
    {
        if ((*child)->getName() == name)
        {
            (*child)->removeFormParent();
            m_children.erase(child);
            delete (*child);
            break;
        }
    }
}

void Node::removeFormParent()
{
    m_parent = nullptr;
}

void Node::removeChildren()
{
    for (const auto child : m_children)
    {
        child->removeFormParent();
        delete child;
    }
    m_children.clear();
}

const glm::mat4 &Node::getTransform()
{
    if (m_dirty)
    {
        genTransform();
        m_dirty = false;
    }

    return m_matTransform;
}

void Node::setDirty(bool dirty)
{
    m_dirty = dirty;
    if(m_dirty)
    {
        for(auto child : m_children)
        {
            child->setDirty(true);
        }
    }
}

void Node::setPosition(const glm::vec3 &pos)
{
    m_pos = pos;
    setDirty(true);
}

void Node::setScale(float scale)
{
    m_scale = scale;
    setDirty(true);
}

void Node::setRotation(float angle, glm::vec3 axis /*= glm::vec3(0., 0., 1.)*/)
{
    m_angle = angle;
    m_rotateAxis = axis;
    setDirty(true);
}

void Node::genTransform()
{
    m_matTransform = glm::mat4(1.0);
    m_matTransform = glm::translate(m_matTransform, m_pos);

    m_matTransform = glm::translate(m_matTransform, glm::vec3(m_size.x * 0.5 * m_scale, m_size.y * 0.5 * m_scale, 0.f));
    m_matTransform = glm::rotate(m_matTransform, glm::radians(m_angle), m_rotateAxis);
    m_matTransform = glm::translate(m_matTransform, glm::vec3(-m_size.x * 0.5 * m_scale, -m_size.y * 0.5 * m_scale, 0.f));

    m_matTransform = glm::scale(m_matTransform, glm::vec3(m_scale, m_scale, 0.f));

    if(m_parent != nullptr)
    {
        m_matTransform *= m_parent->getTransform();
    }
}

void Node::draw(Shader& shader)
{
}
