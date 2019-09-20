#ifndef SPRITE_RENDER_H
#define SPRITE_RENDER_H

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

class SpriteRender
{
public:
	SpriteRender(Shader& Shader);
	~SpriteRender();

	void DrawSprite(Texture2D& texture,
		glm::vec2 pos,
		glm::vec2 size = glm::vec2(10, 10),
		GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f)
		);
private:
	Shader shader;
	GLuint quadVAO;

	void initRenderData();
};

#endif //SPRITE_RENDER_H