#pragma once
#include "texture_base.hpp"

class Texture2D : public TextureBase
{
public:
    ~Texture2D() = default;
    static Texture2D *create();
    static Texture2D *create(int width, int height);
    static Texture2D *create(const std::string &img_path);

    void configTexture(bool isFill = false) override;
    void bindTexture() const override;
    void loadTextureData(const std::string &img_path);

    const std::string getName() const { return m_fileName; }

private:
    Texture2D();

    std::string m_fileName;
};