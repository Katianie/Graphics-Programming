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
#include "KeyboardManager.h"

using namespace glm;

void KeyboardManager::checkUserInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float cameraMoveSpeed;
	static Camera* theCamera;
	vec3 cameraTranslation;

	glfwPollEvents();

	if (glfwGetKey(window, key) != 0)
	{
		ShaderManager &shaderManager = ShaderManager::getInstance();		

		//Button pressed once.
		if(action == GLFW_PRESS)
		{
			//Check to see if user pressed escape key, if so then exit.
			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}

		//Button pressed and/or held.
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theCamera = shaderManager.getCamera();
			cameraMoveSpeed = theCamera->getMoveSpeed();
			cameraTranslation = vec3(0.0f);

			if (key == GLFW_KEY_W)
			{
				cameraTranslation.z = cameraMoveSpeed;
			}
			if (key == GLFW_KEY_A)
			{
				cameraTranslation.x = cameraMoveSpeed;
			}
			if (key == GLFW_KEY_S)
			{
				cameraTranslation.z = -cameraMoveSpeed;
			}
			if (key == GLFW_KEY_D)
			{
				cameraTranslation.x = -cameraMoveSpeed;
			}

			theCamera->setTransformMatrix(translate(theCamera->getTransformMatrix(), cameraTranslation));
		}
	}
}