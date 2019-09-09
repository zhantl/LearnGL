#pragma once

#include "node.hpp"

class Render2d
{
public:
    virtual void submit(const Node *) = 0;
    virtual void flush() = 0;
    // virtual void drawString(const std::string& text, const glm::vec3& pos, const glm::vec4& color){};
};