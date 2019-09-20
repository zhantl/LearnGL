#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "src/shader.h"
#include "src/camera.h"
#include "src/model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>
using namespace std::placeholders;

int winwidth = 800, winheight = 600;
float deltaTime = 0.0f, lastFrame = 0.0f;	// time between current frame and last frame
bool firstMouse = true;
float lastx = 400, lasty = 300;
float fov = 45.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	winwidth = width;
	winheight = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
	return;
	float xoffset = x - lastx;
	float yoffset = y - lasty;
	lastx = x;
	lasty = y;
	if (firstMouse)
	{
		firstMouse = false;
		return;
	}

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double offsetx, double offsety)
{
	return;
	camera.ProcessMouseScroll(offsety);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(winwidth, winheight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glViewport(0, 0, winwidth, winheight);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};

	glEnable(GL_DEPTH_TEST);

	Model myModel("res/objects/rock/rock.obj");
	Shader model_shader("res/shader/model_shader.vs", "res/shader/model_shader.fs");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// be sure to activate shader when setting uniforms/drawing objects
		model_shader.use();
		//model_shader.setVec3("viewPos", camera.Position);

		// directional light
		model_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		model_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05);
		model_shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		model_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		// point light 1
		model_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
		model_shader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		model_shader.setVec3("pointLights[0].diffuse", pointLightColors[0]);
		model_shader.setVec3("pointLights[0].specular", pointLightColors[0]);
		model_shader.setFloat("pointLights[0].constant", 1.0f);
		model_shader.setFloat("pointLights[0].linear", 0.09);
		model_shader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		model_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
		model_shader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		model_shader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
		model_shader.setVec3("pointLights[1].specular", pointLightColors[1]);
		model_shader.setFloat("pointLights[1].constant", 1.0f);
		model_shader.setFloat("pointLights[1].linear", 0.09);
		model_shader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		model_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
		model_shader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		model_shader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
		model_shader.setVec3("pointLights[2].specular", pointLightColors[2]);
		model_shader.setFloat("pointLights[2].constant", 1.0f);
		model_shader.setFloat("pointLights[2].linear", 0.09);
		model_shader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		model_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
		model_shader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
		model_shader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
		model_shader.setVec3("pointLights[3].specular", pointLightColors[3]);
		model_shader.setFloat("pointLights[3].constant", 1.0f);
		model_shader.setFloat("pointLights[3].linear", 0.09);
		model_shader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		model_shader.setVec3("spotLight.position", camera.Position);
		model_shader.setVec3("spotLight.direction", camera.Front);
		model_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		model_shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 1.0f);
		model_shader.setVec3("spotLight.specular", 0.8f, 0.8f, 1.0f);
		model_shader.setFloat("spotLight.constant", 1.0f);
		model_shader.setFloat("spotLight.linear", 0.09);
		model_shader.setFloat("spotLight.quadratic", 0.032);
		model_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		model_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));

		// view/projection transformations
		float zeye = winheight / 1.154700538379252f;
		glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)winwidth / (float)winheight, 10.f, zeye + winheight / 2);
		//glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 view = glm::lookAt(glm::vec3(winwidth * 0.5f, winheight * 0.5f, zeye), glm::vec3(winwidth * 0.5f, winheight * 0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		model_shader.setMat4("projection", projection);
		model_shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.f, 300.f, 509.f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		float angle = glfwGetTime() * 20.0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model_shader.setMat4("model", model);
		myModel.Draw(model_shader);

		auto  test = view * glm::vec4(500.f, 300.f, 509.f, 1.f);
		test = projection *test;
		test /= test.w;

		auto x = (test.x + 1) * 0.5 * winwidth;
		auto y = (1 - test.y) * 0.5 * winheight;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}