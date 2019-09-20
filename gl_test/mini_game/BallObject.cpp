#include "BallObject.h"

BallObject::BallObject() :
Radius(0.f),
Stuck(GL_TRUE),
Sticky(GL_FALSE),
PassThrough(GL_FALSE)
{}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture) :
GameObject(pos, glm::vec2(radius * 2), texture, glm::vec3(1.f), velocity),
Radius(radius),
Stuck(GL_TRUE),
Sticky(GL_FALSE),
PassThrough(GL_FALSE)
{}

glm::vec2 BallObject::Move(GLfloat dt, GLfloat win_w)
{
	if (!this->Stuck)
	{
		this->Position += this->Velocity * dt;
		if (this->Position.x <= 0)
		{
			this->Position.x = 0;
			this->Velocity.x = -this->Velocity.x;
		}
		else if (this->Position.x + this->Size.x >= win_w)
		{
			this->Position.x = win_w - this->Size.x;
			this->Velocity.x = -this->Velocity.x;
		}

		if (this->Position.y <= 0)
		{
			this->Position.y = 0;
			this->Velocity.y = -this->Velocity.y;
		}
	}

	return this->Position;
}

void BallObject::Reset(glm::vec2 pos, glm::vec2 veloctiy)
{
	this->Position = pos;
	this->Velocity = veloctiy;
	this->Stuck = GL_TRUE;
	this->Sticky = GL_FALSE;
	this->PassThrough = GL_FALSE;
	this->Color = glm::vec3(1.f);
}