#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include "GameObject.h"

class BallObject: public GameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;
	GLboolean Sticky, PassThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture);

	glm::vec2 Move(GLfloat dt, GLfloat win_w);
	void      Reset(glm::vec2 pos, glm::vec2 veloctiy);
};

#endif //BALL_OBJECT_H