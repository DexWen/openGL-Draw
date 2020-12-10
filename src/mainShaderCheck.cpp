#include<iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#pragma comment(lib,"FreeImage.lib")
// Other Libs
#include <FI/FreeImage.h>

#include "painter.h"
#include "resourceManager.h"
#define GLM_FORCE_RADIANS

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
using namespace std;

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

Painter dexPainter(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dex-Painter-GOGOGO", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	// glFrontFace(GL_CCW);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Initialize dexPainter
	// dexPainter.Init();
	Shader ourShader = ResourceManager::LoadShader("E:/myGit/openGL-Draw/src/shader/sprite.vs", "E:/myGit/openGL-Draw/src/shader/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
	ourShader.Use().SetInteger("image", 0);
	ourShader.SetMatrix4("projection", projection);

	Texture2D ourTexture = ResourceManager::LoadTexture("E:/myGit/openGL-Draw/src/material/gtaBg2.jpg", GL_TRUE, "bg");

	GLuint VAO, VBO;
	GLfloat vertices[] = {
		0.0f,1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::vec2 position = glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 size = glm::vec2(400, 300);
	GLfloat rotateAngle = 0.0f;
	GLfloat rotate = 3.141592f * rotateAngle / 180.0f;
	glm::vec2 anchor = glm::vec2(0.5f, 0.5f);
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec2 newSize = glm::vec2(size.x * scale.x, size.y * scale.y);
		model = glm::translate(model, glm::vec3(position, 0.0f));
		//rotate = (GLfloat)glfwGetTime() * 1.0f;
		//model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-anchor.x * newSize.x, -anchor.y * newSize.y, 0.0f));

		model = glm::scale(model, glm::vec3(300.0f, 400.0f, 1.0f));

		ourShader.SetMatrix4("model", model);
		ourShader.SetVector3f("spriteColor", color);

		glActiveTexture(GL_TEXTURE0);
		ourTexture.Bind();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	FreeImage_DeInitialise();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	// if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//     glfwSetWindowShouldClose(window, GL_TRUE);
	// if (key >= 0 && key < 1024)
	// {
	//     if (action == GLFW_PRESS)
	//         dexPainter.Keys[key] = GL_TRUE;
	//     else if (action == GLFW_RELEASE)
	// 		dexPainter.Keys[key] = GL_FALSE;
	// }
}