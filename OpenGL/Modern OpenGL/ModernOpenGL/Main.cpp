/** Modern OpenGL
* This program is an example of how to use OpenGL after 2015.
* Many things have changed over the years including libraries,
* and now built in features. This program takes advantage of 
* Vertex Buffer Processing, Shaders, GLEW, GLFW, and more.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2015 Katianie.com
*/
#include "ShaderManager.h"
#include "KeyboardManager.h"
#include "Utils.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define VIEWPORT_WIDTH 1024
#define VIEWPORT_HEIGHT 768

GLFWwindow* initGL()
{
	GLenum error;
	GLFWwindow* theWindow = NULL;
	string errorString;
	Utils &utility = Utils::getInstance(); //Initialize the singleton

	if (glfwInit() == GL_FALSE)
	{
		errorString = "GLFW Error: GLFW Failed to Init.\n\0";
		utility.displayErrorAndExit(theWindow, errorString);
		return NULL;
	}

	//Initialize glfw properties.
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	theWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Eddie O'Hagan", NULL, NULL);
	if (theWindow == NULL)
	{
		errorString = "GLFW Error: GLFW Failed to Create the Window.\n\0";
		utility.displayErrorAndExit(theWindow, errorString);
		return NULL;
	}

	//Make our current window the current context for OpenGL.
	glfwMakeContextCurrent(theWindow);

	//Initialize glew and check for sucessful init.
	error = glewInit();
	if (error != GLEW_OK)
	{
		errorString = "GLEW Error: GLEW failed to init.\n\0";
		utility.displayErrorAndExit(theWindow, errorString);
		return NULL;
	}

	//Enable user input for the window.
	glfwSetInputMode(theWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowTitle(theWindow, "Eddie O'Hagan's OpenGL Demo.\0");

	//Initialize the viewport and GLEW callbacks.
	glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glfwSetErrorCallback(&Utils::errorCallback);
	glfwSetKeyCallback(theWindow, &KeyboardManager::checkUserInput);
	glfwSetWindowCloseCallback(theWindow, &Utils::closeWindowCallback);
	glfwSetFramebufferSizeCallback(theWindow, &Utils::framebufferResizeCallback);

	//Accept fragment if its closer to the camera than the former one.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//Enable alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set the cursor to the center and then hide the cursor.
	glfwSetCursorPos(theWindow, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	glfwSetInputMode(theWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	return theWindow;
}

int main(int argc, char* argv[])
{
	GLuint shaderProgramID;
	GLuint floorTexture;
	GLFWwindow* theWindow;
	Utils &utility = Utils::getInstance();
	ShaderManager &shaderManager = ShaderManager::getInstance();
	KeyboardManager &keyboardManager = KeyboardManager::getInstance();

	//Initialize GLEW and GLFW.
	theWindow = initGL();

	//Initialize the shaders.
	shaderManager.initShaders();

	//Load the shader code into the manager.
	shaderProgramID = shaderManager.loadShaders(theWindow, "Shaders\\VertexShader.glsl", "Shaders\\FragmentShader.glsl");

	//Load the floor texture that will be used to tile the floor.
	floorTexture = utility.loadImageDDS(theWindow, "Textures\\FloorTexture.DDS");

	//Start the main window loop.
	while (glfwWindowShouldClose(theWindow) == 0)
	{
		//Draw code
		shaderManager.drawShaders(theWindow, shaderProgramID, floorTexture);

		//Useing dubble buffering so swap to the other buffer.
		glfwSwapBuffers(theWindow);

		//Check for user input.
		keyboardManager.checkUserInput(theWindow, GLFW_KEY_ESCAPE, GLFW_PRESS, 0, 0);
	}
	
	glDeleteProgram(shaderProgramID);
	glfwDestroyWindow(theWindow);
	glfwTerminate();

	return 0;
}
