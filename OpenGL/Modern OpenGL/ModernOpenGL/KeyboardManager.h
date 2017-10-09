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
#include "stdafx.h"
#include "ShaderManager.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class KeyboardManager
{
public:

	//Singleton is accessed and/or initalized via getInstance()
	static KeyboardManager& getInstance()
	{
		static KeyboardManager instance;
		return instance;
	}

	static void checkUserInput(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	//Private constructor necessary to allow only 1 instance
	KeyboardManager(void)
	{

	}

	virtual ~KeyboardManager()
	{

	}

	KeyboardManager(KeyboardManager const&); // prevent copies
	void operator=(KeyboardManager const&); // prevent assignments
};