//Modified by Christopher Eichert

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderLib/Shader.h>
#include "stb_image.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[]{
0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f
};

float texCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
}

const int stride = 6;

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

	srand(time(NULL));

	//Create vertex buffer, Element Buffer and vertex array object
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array and Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Load data into vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Link color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbind Vertex Array and Vertex Buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Create shader program
	ShaderLib::Shader customShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	////Set border mode and boder color
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	////Set filtering mode
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generate texture holder
	unsigned int texture;
	glGenTextures(1, &texture);

	//Bind to texture holder
	glBindTextures(GL_TEXTURE_2D, texture);

	//Set parameters for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Mipmap settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture image
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/bass.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		//Generate image with previously loaded image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		//Generate mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Failed to load texture");
	}

	//Free data to recover memory
	stbi_image_free(data);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Run Shader program
		customShader.use();

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
		customShader.setMatrix4("_Rotation", rotationMatrix);

		//Clear framebuffer
		glClearColor(backgroundColor, backgroundColor, backgroundColor, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniform to our sin value
		customShader.setFloat("_Time", timeValue);

		customShader.setVector3("_FragmentColors", fragColorX, fragColorY, fragColorZ);

		//Bind new Vertex array to update any change
		glBindVertexArray(VAO);

		//Draw to screen
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}
