#ifndef POWER_UP_H
#define POWER_UP_H

#include "GameObject.h"

const glm::vec2 M_SIZE(60, 20);
const glm::vec2 VELOCITY(0.f, 150.f);

class PowerUp: public GameObject
{
public:
	std::string Type;
	GLfloat		Duration;
	GLboolean	Activated;

	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture):
		GameObject(position, M_SIZE, texture, color, VELOCITY),
		Type(type), Duration(duration), Activated(GL_FALSE)
	{}
};


#endif //POWER_UP_H