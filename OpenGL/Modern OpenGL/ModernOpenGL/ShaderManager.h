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
#include "Camera.h"
#include "Cube.h"
#include "Floor.h"
#include "Utils.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

class ShaderManager
{
public:

	//Singleton is accessed and/or initalized via getInstance()
	static ShaderManager& getInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	//======Public Functions======
	void initShaders();
	GLuint loadShaders(GLFWwindow* window, const char* vertexFilePath, const char* fragmentFilePath);
	void drawShaders(GLFWwindow* window, GLuint programID, GLuint floorTexture);

	//Getters
	Camera* getCamera()
	{
		return myCamera;
	}

private:
	Camera* myCamera;
	Cube* myCube;
	Floor* myFloor;

	//======Constructors and Deconstuctors======
	//Private constructor necessary to allow only 1 instance
	ShaderManager(void) 
	{
		
	}

	virtual ~ShaderManager()
	{
		if (myCamera != NULL)
		{
			delete myCamera;
		}

		if (myCube != NULL)
		{
			delete myCube;
		}

		if (myFloor != NULL)
		{
			delete myFloor;
		}
	}
	
	ShaderManager(ShaderManager const&); //Prevent copies of ShaderManager.
	
	//======Private Functions======
	void checkShaderStatus(GLFWwindow* window, GLuint shaderID, GLenum statusType);
	void loadModelAndCameraTransforms();
	void sendCubeDataToShaders(GLFWwindow* window, GLuint programID, mat4& modelViewProjectionMatrix, mat4& modelMatrix);
	void sendFloorDataToShaders(GLFWwindow* window, GLuint programID, GLuint floorTextureID, mat4& modelViewProjectionMatrix, mat4& modelMatrix);

	//======Operator overloads======
	//Prevent assignments to ShaderManager.
	void operator=(ShaderManager const&); 
};

