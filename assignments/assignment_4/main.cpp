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

void mouse_callback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void processInput(GLFWwindow* window);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

float fieldOfView = 60.0f;

const float SENSITIVITY = 0.1f;

const float CAMERA_SPEED = 5.0f;

const float PITCH_CONSTRAINT = 89.0f;

const int MAX_CUBES = 20;

//Set camera position
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

//Set camera front
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//Set camera target
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Last input mouse position
float lastPosX = SCREEN_WIDTH / 2.0f;
float lastPosY = SCREEN_HEIGHT / 2.0f;

//Delta time 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0;

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

glm::vec3 cubePositions[MAX_CUBES];

float scaleAmount = 2.0f;
float scalarMatrix[16] = {
	scaleAmount, 0, 0, 0,
	0, scaleAmount, 0, 0,
	0, 0, scaleAmount, 0,
	0, 0, 0, 1
};

const int stride = 5;


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Setup Orthographic projection
	glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Cache perspective projection
	glm::mat4 perspectiveView = glm::perspective(glm::radians(fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Set callback for inputs
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int maxCubeRange = 20;
	//Generate random positions for cubes
	for (int i = 0; i < 20; i++) {
		glm::vec3 newPos = glm::vec3(ew::RandomRange(0, maxCubeRange), ew::RandomRange(0, maxCubeRange), ew::RandomRange(0, maxCubeRange));

		cubePositions[i] = newPos;
	}

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
	//GraphicsLib::Shader backgroundShader("assets/Background.vert", "assets/Background.frag");

	GraphicsLib::Shader fishShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	//Create textures
	GraphicsLib::Texture2D waterTexture("assets/WaterBackground.png", 2, 1);

	GraphicsLib::Texture2D fishTexture("assets/Edward.png", 1, 1);

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
    
		//Run Shader program
		fishShader.use();

		//Update shader with new projection matrix
		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
		fishShader.setMatrix4("_Projection", projectionMatrix);

		//Rotate camera to lookat foward position
		glm::mat4 view;
		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		//Update view models
		fishShader.setMatrix4("_View", view);
		fishShader.setMatrix4("_Projection", projectionMatrix);

		//Set uniform to our sin value
		fishShader.setFloat("_Time", timeValue);

		fishShader.setInt("fishTexture", 0);
		// bind Texture
		fishTexture.Bind(GL_TEXTURE0);

		fishShader.setInt("waterTexture", 1);
		waterTexture.Bind(GL_TEXTURE1);
		
		//Render fish
		glBindVertexArray(VAO);
		//Iterate through cube positions and move each to designated position
		for (unsigned int i = 0; i < MAX_CUBES; i++) {
			//Create identy matrix
			glm::mat4 model = glm::mat4(1.0f);

			//Translate to cube position
			model = glm::translate(model, cubePositions[i]);
			
			//Determine angle
			float angle = ((20.0f * i) + 1) * timeValue;

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			fishShader.setMatrix4("_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}

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

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double posX, double posY) {

	if (firstMouse) {
		lastPosX = posX;
		lastPosY = posY;
		firstMouse = false;
	}

	//Calculate offset
	float offsetX = posX - lastPosX;
	float offsetY = lastPosY - posY;

	//Cache current position
	lastPosX = posX;
	lastPosY = posY;

	//Increase value by sensitivity
	offsetX *= SENSITIVITY;
	offsetY *= SENSITIVITY;

	yaw += offsetX;
	pitch += offsetY;

	//Clamp pitch
	if (pitch > PITCH_CONSTRAINT)
		pitch = PITCH_CONSTRAINT;
	if (pitch < -PITCH_CONSTRAINT)
		pitch = -PITCH_CONSTRAINT;

	//Calculate camera rotation
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//Normalize and set camera front to direction
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	fieldOfView -= offsetY;

	if (fieldOfView < 1.0f)
		fieldOfView = 1.0f;
	if (fieldOfView > 60.0f)
		fieldOfView = 60.0f;
}
