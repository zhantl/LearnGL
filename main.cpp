#include <glm/gtc/matrix_transform.hpp>
#include <graphics/shader.hpp>
#include <graphics/window.hpp>
#include <math.h>
#include <time.h>

#include "./src/graphics/render/label.hpp"
#include "./src/graphics/render/sprite.hpp"

#include "./src/camera.h"
#include "./src/graphics/render/sky_box.cpp"

#include "../src/graphics/buffers/frame_buffer.hpp"

int main(int argc, const char **argv)
{
	auto window = Window::getInstance();
	if (!window->initWindow("LearnGL", 960, 540))
	{
		delete window;
		return 0;
	}

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//正交投影用于ui
	glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
	Shader shader_quad("./res/shader/normal.vs", "./res/shader/normal.fs");
	shader_quad.use();
	shader_quad.setMat4("projection", proj);

	Shader shader_lable("./res/shader/normal.vs", "./res/shader/normal_lable.fs");
	shader_lable.use();
	shader_lable.setMat4("projection", proj);

	Shader shader_depth("./res/shader/depth_test.vs", "./res/shader/depth_test.fs");
	shader_depth.use();
	shader_depth.setMat4("projection", proj);

	auto sprite = Sprite::create();
	sprite->setTexture("res/textures/grass.png");
	// sprite->setColor(glm::vec4(0.2, 0.8, 0.3, 1.0));
	// sprite->setPosition(glm::vec3(100., 100., 0.));

	auto lable = Label::create();
	lable->setColor(glm::vec4(1., 0., 0., 1.0));
	lable->setPosition(glm::vec3(10., 500., 0.));
	lable->setScale(0.5);

	FrameBuffer *fb = FrameBuffer::create(960, 540, FrameBuffer::FB_Type::NORMAL);

	//3d 透视投影
	Camera camera(glm::vec3(0., 0., 3.));
	proj = glm::perspective(glm::radians(camera.Zoom), 960.f / 640.f, 0.1f, 100.f);
	std::vector<std::string> imgs = {
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg",
	};
	auto skyBox = SkyBox::create(imgs);
	auto s_shader = skyBox->getShader();
	s_shader->use();
	s_shader->setMat4("projection", proj);

	float start_time = 0.f;
	unsigned int frame = 0;
	while (!window->closed())
	{
		window->clear();

		fb->beginRender();

		glDepthFunc(GL_LEQUAL);
		auto view_m = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		s_shader->use();
		s_shader->setMat4("view", view_m);
		skyBox->draw();
		glDepthFunc(GL_LESS);

		fb->endRender();

		//ui render test
		glDepthMask(GL_FALSE);
		sprite->setTexture(fb->getTexture());
		sprite->draw(shader_quad);
		lable->draw(shader_lable);

		window->update();
		frame++;
		float cur_time = glfwGetTime();
		if (cur_time - start_time > 1.f)
		{
			lable->setString(std::to_string(frame) + " fps");
			start_time = cur_time;
			frame = 0;
		}
	}

	delete sprite;
	delete lable;
	delete skyBox;
	delete window;

	return 0;
}
