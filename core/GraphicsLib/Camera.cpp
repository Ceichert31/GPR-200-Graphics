#include "Camera.h"

namespace GraphicsLib {
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	Camera::Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV) 
	{
		Position = glm::vec3(xPos, yPos, zPos);
		WorldUp = glm::vec3(xUp, yUp, zUp);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	/// <summary>
	/// Calculates the front vector of the camera
	/// </summary>
	void Camera::UpdateCameraVectors()
	{
		//Calculate new front
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		//Calculate new Right
		Right = glm::normalize(glm::cross(Front, WorldUp));  

		//Calculate new Up
		Up = glm::cross(Right, Front);
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;

		if (direction == FOWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;
	}

	void Camera::ProcessMouseMovement(float xOffset, float yOffset)
	{
		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;

		Yaw += xOffset;
		Pitch += yOffset;

		//Clamp pitch
		if (Pitch > PITCH_CONSTRAINT)
			Pitch = PITCH_CONSTRAINT;
		if (Pitch < -PITCH_CONSTRAINT)
			Pitch = -PITCH_CONSTRAINT;

		UpdateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yOffset)
	{
		Fov -= yOffset;

		if (Fov < MIN_FOV)
			Fov = MIN_FOV;
		if (Fov > MAX_FOV)
			Fov = MAX_FOV;
	}
}