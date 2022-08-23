#pragma once
#include <vector>
#include "TextureBase.hpp"

class TextureCube : public TextureBase
{
public:
    ~TextureCube() = default;
    static TextureCube *create();
    static TextureCube *create(const std::vector<std::string> &imgs);

    void configTexture(bool isFill = false) override;
    void bindTexture() const override;
    void fillTexture(const void *data, GLenum pos = GL_TEXTURE_2D) override;

private:
    TextureCube() = default;
    void loadTextureData(const std::vector<std::string> &imgs);
};