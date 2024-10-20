#ifndef CAMERA_H
#define CAMERA_H

#include "../ew/external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GraphicsLib {
	enum CameraMovement {
		FOWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float SENSITIVITY = 0.1f;
	const float FOV = 60.0f;
	const float PITCH_CONSTRAINT = 89.0f;
	const float MAX_FOV = 120.0f;
	const float MIN_FOV = 1.0f;

	class Camera {
		void UpdateCameraVectors();

	public:
		//Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//Euler Angles
		float Yaw;
		float Pitch;

		//Camera Options
		float MovementSpeed;
		float MouseSensitivity;
		float Fov;

		//Euler Angle constructor
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

		//Scalar value constructor
		Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch);

		glm::mat4 GetViewMatrix();

		void ProcessKeyboard(CameraMovement direction, float deltaTime);

		void ProcessMouseMovement(float xOffset, float yOffset);
	
		void ProcessMouseScroll(float yOffset);
	};
}
#endif CAMERA_H