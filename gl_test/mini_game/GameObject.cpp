#include "GameObject.h"

GameObject::GameObject():
	Position(0, 0),
	Size(0, 0),
	Color(1.f),
	Velocity(0.f),
	Rotation(0.0f),
	Destroyed(false),
	IsSolid(false),
	Texture()
{}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color /* = glm::vec3(1.0f) */, glm::vec2 veloctiy /* = glm::vec2(0.f) */) :
	Position(pos),
	Size(size),
	Color(color),
	Rotation(0.0f),
	Velocity(veloctiy),
	Destroyed(false),
	IsSolid(false),
	Texture(texture)
{}

void GameObject::Draw(SpriteRender& render)
{
	render.DrawSprite(this->Texture, this->Position, this->Size, this->Rotation, this->Color);
}