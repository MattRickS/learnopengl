#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "common/shader.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

GLuint LoadVAO(float* vertices, size_t numVertices, unsigned int* indices, size_t numIndices)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, (void*)0);
	glEnableVertexAttribArray(0);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

int main()
{
	// Initialise cross-platform window support using core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Loads the OpenGL function bindings
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ========================================================================
	// Prepare Data

	// Load "mesh" into Vertex Array Object
	float vertices[] {
		 0.5f,  0.5f, 0.0f,  // Top right
		 0.5f, -0.5f, 0.0f,  // Bottom right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left
	};
	unsigned int indices[]{
		0, 1, 3,  // Upper Right
		1, 2, 3   // Lower Left
	};
	GLuint VAO = LoadVAO(vertices, 12, indices, 6);

	// Shaders
	GLuint shaderProgram = LoadShadersProgram("shaders/SimpleVertexShader.glsl", "shaders/SimpleFragmentShader.glsl");

	// ========================================================================
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Initialise new frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		glUseProgram(shaderProgram);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Display and interaction
		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput(window);
	}

	glfwTerminate();
	return 0;
}
