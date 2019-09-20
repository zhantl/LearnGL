#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include <glm/glm.hpp>

struct Particle
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.f), Velocity(0.f), Color(1.f), Life(0.f){}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amout);
	void	Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.f));
	void	Draw();

private:
	std::vector<Particle> particles;
	GLuint amout;
	GLuint VAO;
	Shader shader;
	Texture2D texture;

	void	init();
	GLuint	firstUnuseParticle();
	void	respwanParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.f));
};

#endif //PARTICLE_GENERATOR_H


