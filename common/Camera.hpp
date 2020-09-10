#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

enum class CameraMovement : int
{
	IDLE     = 0,
	RIGHT    = 1,
	LEFT     = 2,
	UP       = 4,
	DOWN     = 8,
	FORWARD  = 16,
	BACKWARD = 32
};

inline CameraMovement operator|(CameraMovement a, CameraMovement b)
{
	return static_cast<CameraMovement>(static_cast<int>(a) | static_cast<int>(b));
}

inline CameraMovement operator&(CameraMovement a, CameraMovement b)
{
	return static_cast<CameraMovement>(static_cast<int>(a) & static_cast<int>(b));
}


class Camera
{
public:
	float yaw, pitch;
	float fov = 45.0f;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float cameraSpeed = 2.5f;
	float mouseSensitivity = 0.1f;
	float scrollSensitity = 2.0f;

	Camera(glm::vec3 pos = glm::vec3(0.0f), float yaw=-90.0f, float pitch=0.0f, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
		: position(pos), yaw(yaw), pitch(pitch), up(up)
	{
		updateAxis();
	};

	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
		: position(pos), forward(glm::normalize(target - pos)), up(up)
	{
		float pitch_radians = asin(forward.y);
		float x = acos(forward.x / cos(pitch_radians));
		yaw = glm::degrees(x);
		pitch = glm::degrees(pitch_radians);
		updateAxis();
	};

	glm::mat4 projectionMatrix(float aspect, float near, float far)
	{
		return glm::perspective(glm::radians(fov), aspect, near, far);
	}

	glm::mat4 viewMatrix()
	{
		return glm::lookAt(position, position + forward, up);
	}

	void processInputMovement(CameraMovement move, float delta)
	{
		float velocity = cameraSpeed * delta;
		if ((bool)(move & CameraMovement::FORWARD))
			position += velocity * forward;
		if ((bool)(move & CameraMovement::BACKWARD))
			position -= velocity * forward;
		if ((bool)(move & CameraMovement::LEFT))
			position -= velocity * right;
		if ((bool)(move & CameraMovement::RIGHT))
			position += velocity * right;
		if ((bool)(move & CameraMovement::UP))
			position += velocity * up;
		if ((bool)(move & CameraMovement::DOWN))
			position -= velocity * up;
	}

	void processInputOffset(float xoffset, float yoffset)
	{
		yaw += xoffset * mouseSensitivity;
		pitch += yoffset * mouseSensitivity;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		updateAxis();
	}

	void processInputZoom(float yoffset)
	{
		fov -= (float)yoffset * scrollSensitity;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}

private:
	void updateAxis()
	{
		glm::vec3 dir = glm::vec3(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		);
		forward = glm::normalize(dir);
		right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, forward));
	}
};


struct CameraInput
{
	float lastX, lastY;
	bool firstInput = true;
	Camera* camera;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	CameraInput* cameraInput = static_cast<CameraInput*>(glfwGetWindowUserPointer(window));

	if (cameraInput->firstInput)
	{
		cameraInput->lastX = (float)xpos;
		cameraInput->lastY = (float)ypos;
		cameraInput->firstInput = false;
	}
	float xoffset = (float)xpos - cameraInput->lastX;
	float yoffset = cameraInput->lastY - (float)ypos;  // y-axis is inverse
	cameraInput->lastX = (float)xpos;
	cameraInput->lastY = (float)ypos;

	// xyoffset is correct

	cameraInput->camera->processInputOffset(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	CameraInput* cameraInput = static_cast<CameraInput*>(glfwGetWindowUserPointer(window));
	cameraInput->camera->processInputZoom(yoffset);
}


// Utiity for setting up the mouse inputs to the camera. Keyboard inputs will still
// need to explicitly call processInputMovement
void attachCamera(GLFWwindow* window, Camera* camera)
{
	if (glfwGetWindowUserPointer(window) != nullptr)
	{
		std::cerr << "Window has already populated user pointer";
		return;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	CameraInput* input = new CameraInput{};
	input->firstInput = true;
	input->camera = camera;

	glfwSetWindowUserPointer(window, (void*)input);

	// How to pass Camera methods as callbacks...?
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}


void detachCamera(GLFWwindow* window)
{
	// If there's no data there's nothing to attach
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr == nullptr)
	{
		std::cerr << "No camera attached to window";
		return;
	}

	// Ensure the pointer is the correct type and then free up the memory
	CameraInput* input = static_cast<CameraInput*>(ptr);
	delete input;

	glfwSetCursorPosCallback(window, 0);
	glfwSetScrollCallback(window, 0);
	glfwSetWindowUserPointer(window, 0);
}