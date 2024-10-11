//Modified by Christopher Eichert

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../core/GraphicsLib/Shader.h"
#include "../../core/GraphicsLib/Texture2D.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

const float FIELD_OF_VIEW = 45.0f;

//Set camera position
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

//Set camera front
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//Set camera target
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Delta time 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//float vertices[] = {
//	// positions          // colors           // texture coords
//		   0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//		   0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//		  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//		  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
//};

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float scaleAmount = 2.0f;
float scalarMatrix[16] = {
	scaleAmount, 0, 0, 0,
	0, scaleAmount, 0, 0,
	0, 0, scaleAmount, 0,
	0, 0, 0, 1
};

//unsigned int indices[] = { 
//	  0, 1, 3,
//	  1, 2, 3
//  };

const int stride = 5;

glm::mat4 CalculateRotationMatrix(float timeValue) {
	//Create matrix
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	//Move to position
	rotationMatrix = glm::translate(rotationMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	float rotationTime = timeValue;

	//Rotation around z-axis
	rotationMatrix = glm::rotate(rotationMatrix, rotationTime, glm::vec3(0.0f, 0.0f, 1.0f));

	return rotationMatrix;
}

const float CAMERA_SPEED = 5.0f;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = CAMERA_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 2.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//Up and down inputs
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPosition += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPosition -= cameraUp * cameraSpeed;
}

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Fish", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
  
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setup Orthographic projection
	glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Cache perspective projection
	glm::mat4 perspectiveView = glm::perspective(glm::radians(FIELD_OF_VIEW), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Rotate plane
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	////Move camera backwards
	//glm::mat4 viewModel = glm::mat4(1.0f);
	//viewModel = glm::translate(viewModel, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Create vertex buffer, Element Buffer and vertex array object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Bind Vertex Array
	glBindVertexArray(VAO);
  
	//Bind and Load data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Bind and Load data in element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Link position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Link UV attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//Create shaders
	GraphicsLib::Shader backgroundShader("assets/Background.vert", "assets/Background.frag");

	GraphicsLib::Shader fishShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	//Create textures
	GraphicsLib::Texture2D backgroundTexture("assets/WaterBackground.png", 2, 1);

	GraphicsLib::Texture2D bubbleTexture("assets/BubbleBackground.png", 3, 1);

	GraphicsLib::Texture2D fishTexture("assets/WaterBackground.png", 1, 1);

	backgroundShader.use();

	backgroundShader.setInt("backgroundTexture", 0);
	backgroundShader.setInt("bubbleTexture", 1);

	backgroundShader.setMatrix4("_ScalarMatrix", glm::make_mat4x4(scalarMatrix));

	const float radius = 10.0f;

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		processInput(window);

		//Clear framebuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calculate delta time
		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;
    
		//Activate shader
		//backgroundShader.use();

		//Set uniforms
		//backgroundShader.setFloat("_Time", timeValue);

		//backgroundShader.setInt("backgroundTexture", 0);
		//backgroundShader.setInt("bubbleTexture", 1);

		//Background textures
		//backgroundTexture.Bind(GL_TEXTURE0);
		//bubbleTexture.Bind(GL_TEXTURE1);

		//Render background
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Run Shader program
		fishShader.use();

		//Calculate direction based on target and position
		glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraDirection);

		//Calculate camera right and camera up
		//glm::vec3 cameraRight = glm::normalize(glm::cross(upVector, cameraDirection));
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		glm::mat4 view;
		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		//Update view models
		fishShader.setMatrix4("_View", view);
		fishShader.setMatrix4("_Projection", projectionMatrix);

		//Set uniform to our sin value
		fishShader.setFloat("_Time", timeValue);

		fishShader.setInt("fishTexture", 2);

		// bind Texture
		fishTexture.Bind(GL_TEXTURE2);
		
		//Render fish
		glBindVertexArray(VAO);
		//Iterate through cube positions and move each to designated position
		for (unsigned int i = 0; i < 10; i++) {
			//Create identy matrix
			glm::mat4 model = glm::mat4(1.0f);

			//Translate to cube position
			model = glm::translate(model, cubePositions[i]);
			
			//Determine angle
			float angle = (20.0f * i) * timeValue;

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			fishShader.setMatrix4("_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}