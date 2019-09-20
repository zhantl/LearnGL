#include "SpriteRender.h"
#include "Game.h"
#include "ResourceManage.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRender		*Renderer;
BallObject			*Ball;
GameObject			*Player;
ParticleGenerator	*ParticleGen;
PostProcessor		*Effect;
GLfloat				ShakeTime = 0.f;

Game::Game(GLuint width, GLuint height):
	State(GAME_ACTIVE),
	Keys(),
	Width(width),
	Height(height)
{
}

Game::~Game()
{
	delete Renderer;
	delete Ball;
	delete Player;
	delete ParticleGen;
	delete Effect;
}

void Game::Init()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	

	//float zeye = this->Height / 1.154700538379252f;
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<GLfloat>(this->Width) / this->Height, 10.0f, zeye + this->Height * 0.5f);
	//glm::vec3 eye(this->Width *0.5f, this->Height * 0.5f, zeye), center(this->Width * 0.5f, this->Height * 0.5f, 0.f), up(0.f, 1.f, 0.f);
	//glm::mat4 lookAt = glm::lookAt(eye, center, up);
	//projection *= lookAt;


	auto ShaderSprite = ResourceManager::LoadShader("res/shader/sprite.vs", "res/shader/sprite.fs", "", "sprite");
	ShaderSprite.Use();
	ShaderSprite.setInt("texture1", 0);
	ShaderSprite.setMat4("projection", projection);
	Renderer = new SpriteRender(ShaderSprite);
	auto ShaderParticle = ResourceManager::LoadShader("res/shader/particle.vs", "res/shader/particle.fs", "", "particle");
	ShaderParticle.Use();
	ShaderParticle.setInt("texture1", 0);
	ShaderParticle.setMat4("projection", projection);
	auto ShaderProcess = ResourceManager::LoadShader("res/shader/post_process.vs", "res/shader/post_process.fs", "", "post_process");

	ResourceManager::LoadTexture("res/image/background.jpg", "background");
	ResourceManager::LoadTexture("res/image/awesomeface.png", "face");
	ResourceManager::LoadTexture("res/image/block.png", "block");
	ResourceManager::LoadTexture("res/image/block_solid.png", "block_solid");
	ResourceManager::LoadTexture("res/image/paddle.png", "paddle");
	ResourceManager::LoadTexture("res/image/particle.png", "particle");
	ResourceManager::LoadTexture("res/image/powerup_chaos.png", "chaos");
	ResourceManager::LoadTexture("res/image/powerup_confuse.png", "confuse");
	ResourceManager::LoadTexture("res/image/powerup_increase.png", "increase");
	ResourceManager::LoadTexture("res/image/powerup_passthrough.png", "passthrough");
	ResourceManager::LoadTexture("res/image/powerup_speed.png", "speed");
	ResourceManager::LoadTexture("res/image/powerup_sticky.png", "sticky");

	GameLevel one; one.Load("res/level_conifg/lv1.txt", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("res/level_conifg/lv2.txt", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("res/level_conifg/lv3.txt", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("res/level_conifg/lv4.txt", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

	ParticleGen = new ParticleGenerator(ShaderParticle, ResourceManager::GetTexture("particle"), 500);

	Effect = new PostProcessor(ShaderProcess, this->Width, this->Height);
}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt, this->Width);
	ParticleGen->Update(dt, *Ball, 2, glm::vec2(Ball->Radius * 0.5f));
	this->DoCollisions();
	this->UpdatePowerUps(dt);

	if (ShakeTime > 0.f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.f)
		{
			Effect->Shake = false;
		}
	}

	if (this->Levels[this->Level].IsCompleted())
	{
		this->Level += 1;
		this->Level %= 4;
		this->ResetLevel();
	}

	if (Ball->Position.y >= this->Height)
	{
		this->ResetLevel();
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		//Effect->BeginRender();
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.f), glm::vec2(this->Width, this->Height), 0.f);
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		for (PowerUp &powerUp : this->PowerUps)
		{
			if (!powerUp.Destroyed)
			{
				powerUp.Draw(*Renderer);
			}
		}
		ParticleGen->Draw();
		Ball->Draw(*Renderer);
		//Effect->EndRender();
		//Effect->Render(glfwGetTime());
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("res/level_conifg/lv1.txt", this->Width, this->Height * 0.5);
	else if (this->Level == 1)
	{
		this->Levels[1].Load("res/level_conifg/lv2.txt", this->Width, this->Height * 0.5);
	}
	else if (this->Level == 2)
	{
		this->Levels[2].Load("res/level_conifg/lv3.txt", this->Width, this->Height * 0.5);
	}
	else if (this->Level == 3)
	{
		this->Levels[2].Load("res/level_conifg/lv4.txt", this->Width, this->Height * 0.5);
	}

	this->PowerUps.clear();

	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);

	Effect->Chaos = GL_FALSE;
	Effect->Confuse = GL_FALSE;
	Effect->Shake = GL_FALSE;
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat veloctiy = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= veloctiy;
				if (Ball->Stuck)
				{
					Ball->Position.x -= veloctiy;
				}
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += veloctiy;
				if (Ball->Stuck)
				{
					Ball->Position.x += veloctiy;
				}
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
		{
			Ball->Stuck = false;
		}

		if (this->Keys[GLFW_KEY_R])
		{
			this->ResetLevel();
		}
	}
}

GLboolean ShouldSpwan(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}

void Game::SpwanPowerUps(GameObject& block)
{
	if (ShouldSpwan(75))
	{
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.f), 0.f, block.Position, ResourceManager::GetTexture("speed")));
	}
	if (ShouldSpwan(75))
	{
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.f, 0.5f, 1.f), 20.f, block.Position, ResourceManager::GetTexture("sticky")));
	}
	if (ShouldSpwan(75))
	{
		this->PowerUps.push_back(PowerUp("passthrough", glm::vec3(0.5f, 1.f, 0.5f), 10.f, block.Position, ResourceManager::GetTexture("passthrough")));
	}
	if (ShouldSpwan(75))
	{
		this->PowerUps.push_back(PowerUp("increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.f, block.Position, ResourceManager::GetTexture("increase")));
	}
	if (ShouldSpwan(2))
	{
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.f, block.Position, ResourceManager::GetTexture("confuse")));
	}
	if (ShouldSpwan(2))
	{
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.f, block.Position, ResourceManager::GetTexture("chaos")));
	}
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp.Activated)
		if (powerUp.Type == type)
			return GL_TRUE;
	}
	return GL_FALSE;
}

void Game::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->PowerUps)
	{
		powerUp.Position += powerUp.Velocity *dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;
			if (powerUp.Duration <= 0.f)
			{
				powerUp.Activated = GL_FALSE;
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.f);
					}
				}
				else if (powerUp.Type == "passthrough")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "passthrough"))
					{
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effect->Confuse = GL_FALSE;
						Ball->Color = glm::vec3(1.f);
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effect->Chaos = GL_FALSE;
					}
				}
			}
		}
	}

	auto iter = this->PowerUps.begin();
	while (iter != this->PowerUps.end())
	{
		if (!iter->Activated && iter->Destroyed)
			iter = this->PowerUps.erase(iter);
		else
			++iter;
	}
}

void ActivatePowerUp(PowerUp &powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "passthrough")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effect->Chaos)
			Effect->Confuse = GL_TRUE;
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effect->Confuse)
			Effect->Chaos = GL_TRUE;
	}
}

Direction	VectorDirection(glm::vec2 target);
GLboolean	CheckCollision(GameObject& one, GameObject& two);
Collision	CheckCollision(BallObject& one, GameObject& two);

void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision))
			{
				if (!box.IsSolid)
				{
					box.Destroyed = GL_TRUE;
					this->SpwanPowerUps(box);
				}	
				else
				{
					Effect->Shake = true;
					ShakeTime = 0.05f;
				}
				Direction dir = std::get<1>(collision);
				glm::vec2 diff = std::get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid))
				{
					if (dir == LEFT || dir == RIGHT)
					{
						Ball->Velocity.x = -Ball->Velocity.x;
						GLfloat dis = Ball->Radius - std::abs(diff.x);
						if (dir == LEFT)
							Ball->Position.x += dis;
						else
							Ball->Position.x -= dis;
					}
					else
					{
						Ball->Velocity.y = -Ball->Velocity.y;
						GLfloat dis = Ball->Radius - std::abs(diff.y);
						if (dir == DOWN)
							Ball->Position.y += dis;
						else
							Ball->Position.y -= dis;
					}
				}
			}
		}
	}
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		Ball->Stuck = Ball->Sticky;
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->Velocity.y = -1 * std::abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
	}
	for (PowerUp& p : this->PowerUps)
	{
		if (!p.Destroyed)
		{
			if (p.Position.y >= this->Height)
				p.Destroyed = GL_TRUE;
			if (CheckCollision(*Player, p))
			{
				ActivatePowerUp(p);
				p.Destroyed = GL_TRUE;
				p.Activated = GL_TRUE;
			}
		}
	}
}


Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // ио
		glm::vec2(1.0f, 0.0f),  // ср
		glm::vec2(0.0f, -1.0f), // об
		glm::vec2(-1.0f, 0.0f)  // вС
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; ++i)
	{
		GLfloat dot_p = glm::dot(glm::normalize(target), compass[i]);
		if (dot_p > max)
		{
			max = dot_p;
			best_match = i;
		}
	}

	std::printf("%d\n", best_match);

	return Direction(best_match);
}

GLboolean CheckCollision(GameObject& one, GameObject& two)
{
	GLboolean cx = one.Position.x + one.Size.x >= two.Position.x && one.Position.x <= two.Position.x + two.Size.x;
	GLboolean cy = one.Position.y + one.Size.y >= two.Position.y && one.Position.y <= two.Position.y + two.Size.y;
	return cx && cy;
}

Collision CheckCollision(BallObject& one, GameObject& two)
{
	glm::vec2 center(one.Position + one.Radius);
	glm::vec2 half_aabb(two.Size.x * 0.5f, two.Size.y * 0.5f);
	glm::vec2 aabb_center(two.Position.x + half_aabb.x, two.Position.y + half_aabb.y);

	glm::vec2 diff = center - aabb_center;
	glm::vec2 clamped = glm::clamp(diff, -half_aabb, half_aabb);

	glm::vec2 closed = aabb_center + clamped;
	diff = closed - center;

	if (glm::length(diff) < one.Radius)
		return Collision(GL_TRUE, VectorDirection(diff), diff);
	else
		return Collision(GL_FALSE, UP, glm::vec2(0, 0));
}