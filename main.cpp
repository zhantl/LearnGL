#include <glm/gtc/matrix_transform.hpp>
#include <graphics/shader.hpp>
#include <graphics/window.hpp>
#include <math.h>
#include <time.h>

#include "./src/graphics/render/label.hpp"
#include "./src/graphics/render/sprite.hpp"

#include "./src/camera.h"
#include "./src/graphics/render/sky_box.hpp"

#include "../src/graphics/buffers/framebuffer.hpp"

#include "./src/graphics/render/cube.hpp"
#include "./src/graphics/render/renderToCube.hpp"
#include "./src/graphics/render/sphere.hpp"

int main(int argc, const char **argv)
{
	auto window = Window::getInstance();
	if (!window->initWindow("LearnGL", 960, 540))
	{
		delete window;
		return 0;
	}

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

	auto sprite = Sprite::create();
	auto lable = Label::create();
	lable->setColor(glm::vec4(1., 0., 0., 1.0));
	lable->setPosition(glm::vec3(10., 500., 0.));
	lable->setScale(0.5);

	//3d 透视投影
	Camera camera(glm::vec3(0., 0., 1.));
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
	auto sphere = Sphere::create();
	auto cube = Cube::create();

	auto renderData = RenderToCube::create();
	Shader shader_equi_to_cube_map("./res/shader/cube_map.vs", "./res/shader/equi_to_cube_map.fs");
	Shader shader_irradiance("./res/shader/cube_map.vs", "./res/shader/irradiance_cube_map.fs");
	Shader shader_prefilter("./res/shader/cube_map.vs", "./res/shader/prefilter_cube_map.fs");
	Shader shader_brdf("./res/shader/normal.vs", "./res/shader/brdf.fs");

	auto hdrTex = Texture2D::create();
	hdrTex->setTextureDataType(GL_FLOAT);
	hdrTex->setInternalFormat(GL_RGB16F);
	hdrTex->loadTextureData("res/textures/hdr/06_kapellen_townsquare2.hdr");
	cube->setTexture(hdrTex);

	auto *fb_base = FrameBuffer::createFrameBuffer(512, 512);
	fb_base->attachDepthRbo(FrameBuffer::Attach::DEPTH24);

	auto cub_tex = TextureCube::create();
	cub_tex->setSize(512, 512);
	cub_tex->setTextureDataType(GL_FLOAT);
	cub_tex->setInternalFormat(GL_RGB16F);
	cub_tex->setFilterMin(GL_LINEAR_MIPMAP_LINEAR);
	cub_tex->setLod(true);
	cub_tex->configTexture(true);
	shader_equi_to_cube_map.use();
	shader_equi_to_cube_map.setMat4("projection", renderData->m_projection);
	for (unsigned int i = 0; i < 6; i++)
	{
		fb_base->attachColor(cub_tex, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
		fb_base->beginRender();
		shader_equi_to_cube_map.setMat4("view", renderData->m_viewMats[i]);
		cube->draw(shader_equi_to_cube_map);
	}
	fb_base->endRender();

	fb_base->resetSize(32, 32);
	cube->setTexture(cub_tex);
	auto irradiance_tex = TextureCube::create();
	irradiance_tex->setSize(32, 32);
	irradiance_tex->setTextureDataType(GL_FLOAT);
	irradiance_tex->setInternalFormat(GL_RGB16F);
	irradiance_tex->configTexture(true);
	shader_irradiance.use();
	shader_irradiance.setMat4("projection", renderData->m_projection);
	for (unsigned int i = 0; i < 6; i++)
	{
		fb_base->attachColor(irradiance_tex, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
		fb_base->beginRender();
		shader_irradiance.setMat4("view", renderData->m_viewMats[i]);
		cube->draw(shader_irradiance);
	}
	fb_base->endRender();

	auto prefilter_tex = TextureCube::create();
	prefilter_tex->setSize(128, 128);
	prefilter_tex->setTextureDataType(GL_FLOAT);
	prefilter_tex->setInternalFormat(GL_RGB16F);
	prefilter_tex->setFilterMin(GL_LINEAR_MIPMAP_LINEAR);
	prefilter_tex->setLod(true);
	prefilter_tex->configTexture(true);
	shader_prefilter.use();
	shader_prefilter.setMat4("projection", renderData->m_projection);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		fb_base->resetSize(mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shader_prefilter.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			shader_prefilter.setMat4("view", renderData->m_viewMats[i]);
			fb_base->attachColor(prefilter_tex, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_COLOR_ATTACHMENT0, mip);
			fb_base->beginRender();
			cube->draw(shader_prefilter);
		}
	}
	fb_base->endRender();

	auto brdf_tex = Texture2D::create();
	brdf_tex->setSize(512, 512);
	brdf_tex->setTextureDataType(GL_FLOAT);
	brdf_tex->setInternalFormat(GL_RG16F);
	brdf_tex->setImageFormat(GL_RG);
	brdf_tex->configTexture(true);

	fb_base->resetSize(512, 512);
	fb_base->attachColor(brdf_tex);
	fb_base->beginRender();
	proj = glm::ortho(0.f, 512.f, 0.f, 512.f, -1.f, 1.f);
	shader_brdf.use();
	shader_brdf.setMat4("projection", proj);
	sprite->setSize(glm::vec2(512, 512));
	sprite->draw(shader_brdf);
	fb_base->endRender();

	// skyBox->setTexture(cub_tex);

	auto albedoMap = Texture2D::create("./res/textures/pbr/gold/albedo.png");
	auto normalMap = Texture2D::create("./res/textures/pbr/gold/normal.png");
	auto metallicMap = Texture2D::create("./res/textures/pbr/gold/metallic.png");
	auto roughnessMap = Texture2D::create("./res/textures/pbr/gold/roughness.png");
	auto aoMap = Texture2D::create("./res/textures/pbr/gold/ao.png");

	Shader shader_pbr("./res/shader/pbr_base.vs", "./res/shader/pbr_base.fs");
	shader_pbr.use();
	shader_pbr.setInt("albedoMap", 0);
	shader_pbr.setInt("normalMap", 1);
	shader_pbr.setInt("metallicMap", 2);
	shader_pbr.setInt("roughnessMap", 3);
	shader_pbr.setInt("aoMap", 4);
	shader_pbr.setInt("irradianceMap", 5);
	shader_pbr.setInt("prefilterMap", 6);
	shader_pbr.setInt("brdfLUT", 7);

	// lights
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f, 10.0f, 10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
	};

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	float start_time = 0.f;
	unsigned int frame = 0;
	while (!window->closed())
	{
		window->clear();

		proj = glm::perspective(glm::radians(camera.Zoom), 960.f / 640.f, 0.1f, 100.f);
		auto view_m = camera.GetViewMatrix();

		glDepthFunc(GL_LESS);
		shader_pbr.use();
		shader_pbr.setMat4("projection", proj);
		shader_pbr.setMat4("view", view_m);
		shader_pbr.setVec3("viewPos", camera.Position);

		glActiveTexture(GL_TEXTURE0);
		albedoMap->bindTexture();
		glActiveTexture(GL_TEXTURE1);
		normalMap->bindTexture();
		glActiveTexture(GL_TEXTURE2);
		metallicMap->bindTexture();
		glActiveTexture(GL_TEXTURE3);
		roughnessMap->bindTexture();
		glActiveTexture(GL_TEXTURE4);
		aoMap->bindTexture();
		glActiveTexture(GL_TEXTURE5);
		irradiance_tex->bindTexture();
		glActiveTexture(GL_TEXTURE6);
		prefilter_tex->bindTexture();
		glActiveTexture(GL_TEXTURE7);
		brdf_tex->bindTexture();

		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				sphere->setScale(1.f);
				sphere->setPosition(glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, -2.0f));
				sphere->draw(shader_pbr);
			}
		}

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			shader_pbr.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			shader_pbr.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			sphere->setScale(0.5f);
			sphere->setPosition(newPos);
			sphere->draw(shader_pbr);
		}

		glDepthFunc(GL_LEQUAL);
		s_shader->use();
		s_shader->setMat4("projection", proj);
		s_shader->setMat4("view", view_m);
		skyBox->draw();

		// ui render
		glDepthMask(GL_FALSE);

		// sprite->setTexture(brdf_tex);
		// sprite->draw(shader_quad);
		lable->draw(shader_lable);

		glDepthMask(GL_TRUE);

		frame++;
		float cur_time = glfwGetTime();
		if (cur_time - start_time > 1.f)
		{
			lable->setString(std::to_string(frame) + " fps");
			start_time = cur_time;
			frame = 0;
		}

		window->update();
	}

	delete sprite;
	delete lable;
	delete skyBox;
	delete window;

	return 0;
}
