#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "SpriteRender.h"
#include "GameObject.h"
#include <vector>

class GameLevel
{
public:
	std::vector<GameObject> Bricks;

	void	  Load(const std::string& file, GLuint LvWidth, GLuint LvHeight);
	void	  Draw(SpriteRender& render);
	GLboolean IsCompleted();
private:
	void	  Init(std::vector<std::vector<GLuint>> tileData, GLuint LvWidth, GLuint LvHeight);
};

#endif //GAME_LEVEL_H