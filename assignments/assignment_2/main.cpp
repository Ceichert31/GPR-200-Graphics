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

float vertices[] = {
	// positions          // colors           // texture coords
		   0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		   0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

float scaleAmount = 2.0f;
float scalarMatrix[16] = {
	scaleAmount, 0, 0, 0,
	0, scaleAmount, 0, 0,
	0, 0, scaleAmount, 0,
	0, 0, 0, 1
};

unsigned int indices[] = { 
	  0, 1, 3,
	  1, 2, 3
  };

const int stride = 8;

glm::mat4 CalculateRotationMatrix(float timeValue) {
	//Create matrix
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	//Move to position
	rotationMatrix = glm::translate(rotationMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	float rotationTime = sin(timeValue);

	//Rotation around z-axis
	rotationMatrix = glm::rotate(rotationMatrix, rotationTime, glm::vec3(0.0f, 0.0f, 2.0f));

	return rotationMatrix;
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Bind and Load data in element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Link position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Link color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//Link UV attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	//Create shaders
	ShaderLib::Shader backgroundShader("assets/Background.vert", "assets/Background.frag");

	ShaderLib::Shader fishShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	//Create textures
	ShaderLib::Texture2D backgroundTexture("assets/WaterBackground.png", 1, 1);

	ShaderLib::Texture2D bubbleTexture("assets/BubbleBackground.png", 1, 1);

	ShaderLib::Texture2D fishTexture("assets/Edward.png", 1, 1);

	backgroundShader.use();

	backgroundShader.setInt("backgroundTexture", 0);
	backgroundShader.setInt("bubbleTexture", 1);

	backgroundShader.setMatrix4("_ScalarMatrix", glm::make_mat4x4(scalarMatrix));

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Clear framebuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
    
		//Activate shader
		backgroundShader.use();

		//Set uniforms
		backgroundShader.setFloat("_Time", timeValue);

		backgroundShader.setInt("backgroundTexture", 0);
		backgroundShader.setInt("bubbleTexture", 1);

		//Background textures
		backgroundTexture.Bind(GL_TEXTURE0);
		bubbleTexture.Bind(GL_TEXTURE1);

		//Render background
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Run Shader program
		fishShader.use();

		//Rotate
		fishShader.setMatrix4("_Rotation", CalculateRotationMatrix(timeValue));

		//Set uniform to our sin value
		fishShader.setFloat("_Time", timeValue);

		fishShader.setInt("ourTexture", 2);

		// bind Texture
		fishTexture.Bind(GL_TEXTURE2);
		
		//Render fish
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}
}

glm::mat4 CalculateRotationMatrix(float timeValue) {
	//Create matrix
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	//Move to position
	rotationMatrix = glm::translate(rotationMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	float rotationTime = sin(timeValue);

	//Rotation around z-axis
	rotationMatrix = glm::rotate(rotationMatrix, rotationTime, glm::vec3(0.0f, 0.0f, 2.0f));

	return rotationMatrix;
}
