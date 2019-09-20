#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "SpriteRender.h"
#include "Texture.h"
#include <glm/glm.hpp>

class GameObject
{
public:
	glm::vec2 Position, Size, Velocity;
	GLfloat Rotation;
	glm::vec3 Color;
	GLboolean IsSolid;
	GLboolean Destroyed;

	Texture2D Texture;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f), glm::vec2 veloctiy = glm::vec2(0.f));

	virtual void Draw(SpriteRender& render);
};

#endif //GAME_OBJECT_H
