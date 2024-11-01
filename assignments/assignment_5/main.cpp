//Modified by Christopher Eichert

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../../core/GraphicsLib/Shader.h"
#include "../../core/GraphicsLib/Texture2D.h"
#include "../../core/GraphicsLib/Camera.h"

//Function Prototypes
void mouse_callback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void processInput(GLFWwindow* window);

//Screen Settings
GraphicsLib::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

//Last input mouse position
float lastPosX = SCREEN_WIDTH / 2.0f;
float lastPosY = SCREEN_HEIGHT / 2.0f;

//Delta time 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices[] = {
		//Px    Py     Pz     Nx     Ny     Nz     U     V
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

//Max Cube Settings
const int MAX_CUBES = 30;
int maxCubeRange = 20;
float maxCubeScale = 1.5f;

//Vertex Attrib Settings
const int STRIDE = 8;

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation; //Euler Angles
	glm::vec3 scale;
	glm::mat4 getModelMatrix() {
		//Create identity Matrix
		glm::mat4 model = glm::mat4(1.0f);

		//Translation model
		model = glm::translate(model, position);

		//Rotation model
		float angle = ((20.0f) * glfwGetTime());
		model = glm::rotate(model, glm::radians(angle), rotation);

		//Scale model
		model = glm::scale(model, scale);

		return model;
	}
};

//Transform transforms[MAX_CUBES];
//Transforms
Transform cube;
Transform lightTransform;

//Light Settings
glm::vec3 lightColor = glm::vec3(1.0f);
float ambientIntensity = 0.3f;
float diffuseIntensity = 0.0f;
float specularIntensity = 0.5f;
float shininessIntensity = 32.0f;
bool blinnPhong = true;

bool canLook = false;

void InstantiateCubes() {
	//Generate random model matrix values for cubes
	/*for (int i = 0; i < MAX_CUBES; i++) {
		transforms[i].position = glm::vec3(ew::RandomRange(0, maxCubeRange), ew::RandomRange(0, maxCubeRange), ew::RandomRange(0, maxCubeRange));

		transforms[i].rotation = glm::vec3(ew::RandomRange(0, 1), ew::RandomRange(0, 1), ew::RandomRange(0, 1));

		transforms[i].scale = glm::vec3(ew::RandomRange(0.5f, maxCubeScale), ew::RandomRange(0.5f, maxCubeScale), ew::RandomRange(0.5f, maxCubeScale));
	}*/
}

void ClearCubes() {
	//Generate random model matrix values for cubes
	/*for (int i = 0; i < MAX_CUBES; i++) {
		transforms[i].position = glm::vec3(0, 0, 0);

		transforms[i].rotation = glm::vec3(0, 0, 0);

		transforms[i].scale = glm::vec3(0, 0, 0);
	}*/
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
	glm::mat4 perspectiveView = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
	glm::mat4 projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

	//Set callback for inputs
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Spawn cubes
	InstantiateCubes();

	//Create vertex buffer, Element Buffer and vertex array object
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array
	glBindVertexArray(VAO);
  
	//Bind and Load data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Link position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));

	//Link UV attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Light Source
	unsigned int lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Create shaders
	GraphicsLib::Shader lightingShader("assets/VertexShader.vert", "assets/FragmentShader.frag");
	GraphicsLib::Shader lampShader("assets/Lamp.vert", "assets/Lamp.frag");

	//Create textures
	GraphicsLib::Texture2D waterTexture("assets/WaterBackground.png", 2, 1);
	GraphicsLib::Texture2D fishTexture("assets/Edward.png", 1, 1);

	//Lighting
	cube.position = glm::vec3(2.0f);
	cube.rotation = glm::vec3(1.0f);
	cube.scale = glm::vec3(1.0f);
	lightTransform.position = glm::vec3(1.0f);
	lightTransform.rotation = glm::vec3(2.0f);
	lightTransform.scale = glm::vec3(0.4f);
	
	//Initialize IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Render loop
	while (!glfwWindowShouldClose(window)) {


		processInput(window);

		//Calculate delta time
		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Run Shader program
		lightingShader.use();

		lightingShader.setVector3("lightColor", lightColor);
		lightingShader.setVector3("lightPos", lightTransform.position);
		lightingShader.setVector3("viewPos", camera.Position);
		lightingShader.setFloat("ambientStrength", ambientIntensity);
		lightingShader.setFloat("diffuseStrength", diffuseIntensity);
		lightingShader.setFloat("specularStrength", specularIntensity);
		lightingShader.setFloat("shininess", shininessIntensity);
		lightingShader.setBool("blinnPhong", blinnPhong);

		//Calculate new projection matrix
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

		//Calculate new view matrix
		glm::mat4 view = camera.GetViewMatrix();

		//Update view models
		lightingShader.setMatrix4("_View", view);
		lightingShader.setMatrix4("_Projection", projectionMatrix);

		//Set uniform time to delta time
		lightingShader.setFloat("_Time", deltaTime);

		//Bind Textures
		lightingShader.setInt("fishTexture", 0);
		fishTexture.Bind(GL_TEXTURE0);
		lightingShader.setInt("waterTexture", 1);
		waterTexture.Bind(GL_TEXTURE1);
		 
		lightingShader.setMatrix4("_Model", cube.getModelMatrix());

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.use();

		lampShader.setVector3("lightColor", lightColor);

		lampShader.setMatrix4("_View", view);
		lampShader.setMatrix4("_Projection", projectionMatrix);

		lightTransform.position.y += sin(timeValue * 2.0f) * deltaTime;
		lightTransform.rotation = cube.position;
		lampShader.setMatrix4("_Model", lightTransform.getModelMatrix());
		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Iterate through cube positions and move each to designated position
		//for (unsigned int i = 0; i < MAX_CUBES; i++) {
		//	//Translate to cube position
		//	transforms[i].position.y += (sin(timeValue) * 2.0f) * deltaTime;

		//	lightingShader.setMatrix4("_Model", transforms[i].getModelMatrix());
		//	
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		//Create a window
		ImGui::Begin("Lighting Settings");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::DragFloat3("Light Position", &lightTransform.position.x);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("AmbientK", &ambientIntensity, 0, 1);
		ImGui::SliderFloat("DiffuseK", &diffuseIntensity, 0, 1);
		ImGui::SliderFloat("SpecularK", &specularIntensity, 0, 1);
		ImGui::SliderFloat("Shininess", &shininessIntensity, 2, 1024);
		ImGui::Checkbox("Blinn-Phong", &blinnPhong);
		ImGui::End();

		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &VBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
bool firstMouse = true;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Hold rightclick to look
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		canLook = true;
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		canLook = false;
		firstMouse = true;
	}

	if (!canLook) return;

	//Sprinting
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		deltaTime *= 2;

	//Standard inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::FOWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::RIGHT, deltaTime);

	//Up and down inputs
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(GraphicsLib::DOWN, deltaTime);

	//Creating and deleting cubes
	/*if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		InstantiateCubes();
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		ClearCubes();*/
}



void mouse_callback(GLFWwindow* window, double posX, double posY) {
	if (!canLook) return;
	
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

	camera.ProcessMouseMovement(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	if (!canLook) return;

	camera.ProcessMouseScroll(offsetY);
}
