//Modified by Christopher Eichert

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../core/ShaderLib/Shader.h"
#include "../../core/ShaderLib/Texture2D.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[]{
	// positions          // colors           // texture coords
		   0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		   0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = { 
	  0, 1, 3,
	  1, 2, 3
};

const int stride = 8;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Create vertex buffer, Element Buffer and vertex array object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Bind Vertex Array and Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Load data into vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Link color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//Link UV attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind Vertex Array and Vertex Buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Create shader program

	ShaderLib::Shader backgroundShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	ShaderLib::Shader fishShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	ShaderLib::Texture2D fishTexture("assets/Edward.png", 1, 1);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		

		//Run Shader program
		fishShader.use();

		// bind Texture
		fishTexture.Bind(0);

		//Calculate sin value and get the current uniform information
		float timeValue = glfwGetTime();

		float fragColorX = cos(timeValue);

		float fragColorY = cos(timeValue);

		float fragColorZ = cos(timeValue);

		float backgroundColor = sin(timeValue) + 1;

		//Create matrix
		glm::mat4 rotationMatrix = glm::mat4(1.0f);

		//Move to position
		rotationMatrix = glm::translate(rotationMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		float rotationTime = sin(timeValue);

		//Rotation around z-axis
		rotationMatrix = glm::rotate(rotationMatrix, rotationTime, glm::vec3(0.0f, 0.0f, 2.0f));

		//Rotate
		fishShader.setMatrix4("_Rotation", rotationMatrix);

		//Clear framebuffer
		glClearColor(backgroundColor, backgroundColor, backgroundColor, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniform to our sin value
		fishShader.setFloat("_Time", timeValue);

		fishShader.setVector3("_FragmentColors", fragColorX, fragColorY, fragColorZ);

		//Bind new Vertex array to update any change
		glBindVertexArray(VAO);

		//Draw to screen
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}