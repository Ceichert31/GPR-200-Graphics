#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[]{
0.5f, 0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
-0.5f, -0.5f, 0.0f,
//-0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	//1, 2, 3
};

//Cache shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


//Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


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
	//Create a vertex shader 
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Generate vertex shader with GL methods
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//Compile shader
	glCompileShader(vertexShader);

	//Debug result
	int vertexShaderCompiled;
	char vertexInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);

	//Compile error check
	if (!vertexShaderCompiled) {
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
		printf("ERROR:SHADER:VERTEX:COMPILATION_FAILED\n%s", vertexInfoLog);
	}
	
	//Allocate new fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Assign fragment shader source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//Compile
	glCompileShader(fragmentShader);

	//Fragment compile error check
	int fragmentShaderCompiled;
	char fragmentInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);

	if (!fragmentShader) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
		printf("ERROR:SHADER:VERTEX:COMPILATION_FAILED\n%s", vertexInfoLog);
	}

	//Create a new shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attach previous shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for compilation errors
	int shaderProgramCompiled;

	char shaderInfoLog[512];

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &shaderProgramCompiled);

	if (!shaderProgramCompiled) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);
		printf("ERROR:SHADER:PROGRAM:LINKING_FAILED\n%s", shaderInfoLog);
	}

	//Deallocate shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Create vertex buffer and vertex array object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array and Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Load data into vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind Vertex Array and Vertex Buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Run Shader Program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	return 0;
}
