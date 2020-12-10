#include<iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#pragma comment(lib,"FreeImage.lib")

// Other Libs
#include <other/Shader.h>
#include <FI/FreeImage.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
using namespace std;

const GLuint VIEW_WIDTH = 800, VIEW_HEIGHT = 600;
int main() {

    // ==========  init ===========
	glfwInit();
	FreeImage_Initialise(TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, "Dex-View", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ======================= //

	Shader ourShader("E:/myGit/openGL-Draw/src/shader/shaderBase.vs", "E:/myGit/openGL-Draw/src/shader/shaderBase.frag");
	GLfloat vertices[] = {
		0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,

		0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 0.6f,
		1.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.6f,
		0.0f,  -0.5f,  0.0f, 1.0f, 0.0f, 0.6f,
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 6
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //

	int width, height;

	const char* imagePath = "E:/myGit/openGL-Draw/src/material/gtaBg2.jpg";
	FIBITMAP* dib(0);
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(imagePath, 0);

	if (FreeImage_FIFSupportsReading(fifmt))
		dib = FreeImage_Load(fifmt, imagePath, 0);

	  //if the image failed to load, return failure
	if (!dib) {
		printf("read Image Error");
		return 0;
	}


	dib = FreeImage_ConvertTo24Bits(dib);

	BYTE* pixels = (BYTE*)FreeImage_GetBits(dib);

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	cout << "width:" << width << endl;
	cout << "height:" << height << endl;


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 


	// Set the texture wrapping parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);


	FreeImage_Unload(dib);
	FreeImage_DeInitialise();


	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); //

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glBindTexture(GL_TEXTURE_2D, texture);
		ourShader.Use();

		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
