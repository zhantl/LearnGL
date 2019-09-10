#include <time.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/shader.hpp>
#include <graphics/window.hpp>

#include "./src/graphics/render/label.hpp"
#include "./src/graphics/render/sprite.hpp"


int main(int argc, const char **argv)
{
	Window window("LearnGL", 960, 540);
	// glClearColor(1.f, 1.f, 1.f, 1.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
	Shader shader("./res/shader/normal.vs", "./res/shader/normal.fs");
	shader.use();
	shader.setMat4("proj", proj);

	Shader shader_lable("./res/shader/normal.vs", "./res/shader/normal_lable.fs");
	shader_lable.use();
	shader_lable.setMat4("proj", proj);

	auto sprite = Sprite::create();
	sprite->setTexture("res/textures/container2.png");
	sprite->setColor(glm::vec4(0.2, 0.8, 0.3, 1.0));
	sprite->setScale(0.5);

	auto sprite2 = Sprite::create("res/textures/grass.png");
	sprite2->setColor(glm::vec4(0., 1., 1., 1.0));
	sprite2->setPosition(glm::vec3(100., 0., 0.));
	sprite->addChild(sprite2);

	auto lable = Label::create();
	lable->setColor(glm::vec4(1., 0., 0., 1.0));
	lable->setPosition(glm::vec3(200., 150., 0.));
	lable->setString("怎么回事？");
	sprite->addChild(lable);

	float start_time = 0.f;
	unsigned int frame = 0;
	float angel = 1.0;
	glm::vec3 pos = glm::vec3(0., 0., 0.);
	while (!window.closed())
	{
		window.clear();

		double x, y;
		window.getCursorPosition(x, y);
		shader.setVec2("light_pos", glm::vec2((float)x, 540.f - (float)y));
		angel += 0.1;
		pos.x += 1.0;
		pos.y += 1.0;
		if (pos.x > window.getWidth())
		{
			pos.x = 0.;
		}
		if (pos.y > window.getHeight())
		{
			pos.y = 0.;
		}
		sprite->setRotation(angel);
		sprite->setPosition(glm::vec3(200., 200., 0.));
		sprite->draw(shader);
		sprite2->draw(shader);

		lable->draw(shader_lable);

		window.update();
		frame++;
		float cur_time = glfwGetTime();
		if (cur_time - start_time > 1.f)
		{
			std::cout << frame << " fps" << std::endl;
			start_time = cur_time;
			frame = 0;
		}
	}

	return 0;
}
