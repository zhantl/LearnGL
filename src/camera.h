#ifndef CAMERA_H
#define CAMERA_H

#include "graphics/event_dispatch.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.4f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		init();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		init();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(const Event &event)
	{
		auto key = *static_cast<int *>(event.getArgs());
		float velocity = MovementSpeed * 0.033;
		if (key == GLFW_KEY_W)
			Position += Front * velocity;
		if (key == GLFW_KEY_S)
			Position -= Front * velocity;
		if (key == GLFW_KEY_A)
			Position -= Right * velocity;
		if (key == GLFW_KEY_D)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(const Event &event)
	{
		auto pos = static_cast<glm::vec2 *>(event.getArgs());
		auto offset_x = pos->x - last_pos.x;
		auto offset_y = pos->y - last_pos.y;
		offset_x *= MouseSensitivity;
		offset_y *= MouseSensitivity;
		last_pos.x = pos->x;
		last_pos.y = pos->y;

		Yaw += offset_x;
		Pitch += offset_y;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(const Event &event)
	{
		auto offset = static_cast<glm::vec2 *>(event.getArgs());
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= offset->y;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	glm::vec2 last_pos;
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void init()
	{
		last_pos.x = 480;
		last_pos.y = 270;
		auto event_ls_scroll = new EventListener(EventType::EVENT_SCROLL, std::bind(&Camera::ProcessMouseScroll, this, std::placeholders::_1));
		EventDispatch::getInstance()->addEventListener(event_ls_scroll);

		auto evet_ls_touch = new EventListener(EventType::EVENT_TOUCH, std::bind(&Camera::ProcessMouseMovement, this, std::placeholders::_1));
		EventDispatch::getInstance()->addEventListener(evet_ls_touch);

		auto evet_ls_key = new EventListener(EventType::EVNET_KEYBOARD, std::bind(&Camera::ProcessKeyboard, this, std::placeholders::_1));
		EventDispatch::getInstance()->addEventListener(evet_ls_key);
	}
};
#endif