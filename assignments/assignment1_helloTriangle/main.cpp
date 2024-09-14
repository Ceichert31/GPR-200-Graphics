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

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[]{
0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f
};

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
