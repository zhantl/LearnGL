#include "SpriteRender.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRender::SpriteRender(Shader& Shader) : shader(Shader)
{
	this->initRenderData();
}

SpriteRender::~SpriteRender()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRender::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Î»ÖÃ     // ÎÆÀí
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRender::DrawSprite(Texture2D& texture, glm::vec2 pos, 
	glm::vec2 size /* = glm::vec2(10 , 10)*/, GLfloat rotate /* = 0.0f */, 
	glm::vec3 color /* = glm::vec3(1.0f) */)
{
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, -600 / 1.154700538379252f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));
	this->shader.setMat4("model", model);
	this->shader.setVec3("color", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}