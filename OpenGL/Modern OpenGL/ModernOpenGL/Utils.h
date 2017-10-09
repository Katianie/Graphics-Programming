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
#pragma once
#include "stdafx.h"

//Used for DDS Image decompression. Depending on the flag, diffrent compressions exist.
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class Utils 
{
public:

	//Singleton is accessed and/or initalized via getInstance()
	static Utils& getInstance()
	{
		static Utils instance;
		return instance;
	}
	
	//Util functions
	void displayErrorAndExit(GLFWwindow* window, string errorString);
	char* readFile(GLFWwindow* window, const char* filepath);
	GLuint loadImageDDS(GLFWwindow* window, const char * imagepath);

	//GLFW Callbacks
	static void Utils::closeWindowCallback(GLFWwindow* window)
	{
		if (window != NULL)
		{
			glfwDestroyWindow(window);
		}

		glfwTerminate();
	}
	static void Utils::errorCallback(int error, const char* description)
	{
		fprintf(stderr, "Error (%d): %s\n\0", error, description);
	}
	static void Utils::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		glfwSetCursorPos(window, width / 2, height / 2);
	}

private:
	//Private constructor necessary to allow only 1 instance
	Utils(void)
	{
	}

	virtual ~Utils()
	{

	}

	Utils(Utils const&); // prevent copies
	void operator=(Utils const&); // prevent assignments
};


