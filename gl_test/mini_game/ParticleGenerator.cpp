#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amout):
shader(shader),
texture(texture),
amout(amout)
{
	this->init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset /* = glm::vec2(0.f) */)
{
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedidx = this->firstUnuseParticle();
		this->respwanParticle(this->particles[unusedidx], object, offset);
	}

	for (GLuint i = 0; i < this->amout; ++i)
	{
		Particle& p = this->particles[i];
		p.Life -= dt;
		if (p.Life > 0.f)
		{
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5f;
		}
	}
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle p : this->particles)
	{
		if (p.Life > 0.f)
		{
			this->shader.setVec2("offset", p.Position);
			this->shader.setVec4("color", p.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->amout; ++i)
	{
		this->particles.push_back(Particle());
	}
}

GLuint lastUserdParticle = 0;
GLuint ParticleGenerator::firstUnuseParticle()
{
	for (GLuint i = lastUserdParticle; i < this->amout; ++i)
	{
		if (this->particles[i].Life <= 0.f)
		{
			lastUserdParticle = i;
			return i;
		}
	}

	for (GLuint i = 0; i < lastUserdParticle; ++i)
	{
		if (this->particles[i].Life <= 0.f)
		{
			lastUserdParticle = i;
			return i;
		}
	}

	lastUserdParticle = 0;
	return 0;
}

void ParticleGenerator::respwanParticle(Particle& particle, GameObject& object, glm::vec2 offset /* = glm::vec2(0.f) */)
{
	GLfloat random = ((rand() % 100) - 50) / 10.f;
	GLfloat rColor = 0.5f + ((rand() % 100) / 100.f);
	particle.Position = object.Position + offset + random;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.f);
	particle.Life = 1.f;
	particle.Velocity = object.Velocity * 0.1f;
}

