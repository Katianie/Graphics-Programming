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

void ShaderManager::initShaders()
{
	myCamera = new Camera();
	myCube = new Cube();
	myFloor = new Floor();
}

//Checks OpenGL to see if our shaders have compiled properly or h
void ShaderManager::checkShaderStatus(GLFWwindow* window, GLuint shaderID, GLenum statusType)
{
	int infoLogLength;
	GLint compileResult = GL_FALSE;
	GLchar* compileInfoLog;
	Utils &utility = Utils::getInstance();

	glGetShaderiv(shaderID, statusType, &compileResult);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	//Check if there are results in the log
	if (infoLogLength > 0)
	{
		//Allocate space for the log and retrieve the info.
		compileInfoLog = (char*)calloc(infoLogLength + 1, sizeof(char));
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, compileInfoLog);

		utility.displayErrorAndExit(window, compileInfoLog);
	}
}

//Sends all of the vertex, normal, uv, and texture data to the shader for the rotating cube.
void ShaderManager::sendCubeDataToShaders(GLFWwindow* window, GLuint programID, mat4& modelViewProjectionMatrix, mat4& modelMatrix)
{
	//Obtain handles to the variables in the shader code.
	GLuint vertexPosInModelSpaceID = glGetAttribLocation(programID, "vertexPosInModelSpace");
	GLuint modelViewProjectionMatrixID = glGetUniformLocation(programID, "modelViewProjectionMatrix");
	GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");
	mat4 pipelineMatrix = modelViewProjectionMatrix * modelMatrix;

	//Pass the ModelViewProjection matrix to the shaders.
	glUniformMatrix4fv(modelViewProjectionMatrixID, 1, GL_FALSE, &pipelineMatrix[0][0]);

	//Enable the vertex buffer. Specifies the location and data format of the vertex buffer.
	glEnableVertexAttribArray(vertexPosInModelSpaceID);
	glBindBuffer(GL_ARRAY_BUFFER, myCube->getVertexBufferID());
	glVertexAttribPointer(vertexPosInModelSpaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Enable the color buffer. Specifies the location and data format of the color buffer.
	glEnableVertexAttribArray(vertexColorID);
	glBindBuffer(GL_ARRAY_BUFFER, myCube->getColorBufferID());
	glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myCube->getIndexBufferID());
	glDrawElements(GL_TRIANGLES, sizeof(myCube->cube_indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	//Disable the use of vertex and color buffers;
	glDisableVertexAttribArray(vertexPosInModelSpaceID);
	glDisableVertexAttribArray(vertexColorID);
}

//Sends all of the vertex, normal, uv, and texture data to the shader for the floor.
void ShaderManager::sendFloorDataToShaders(GLFWwindow* window, GLuint programID, GLuint floorTextureID, mat4& modelViewProjectionMatrix, mat4& modelMatrix)
{
	//Obtain handles to the variables in the shader code.
	GLuint vertexPosInModelSpaceID = glGetAttribLocation(programID, "vertexPosInModelSpace");
	GLuint modelViewProjectionMatrixID = glGetUniformLocation(programID, "modelViewProjectionMatrix");
	GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
	GLuint textureSamplerID = glGetUniformLocation(programID, "textureSampler");
	mat4 pipelineMatrix = modelViewProjectionMatrix * modelMatrix;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTextureID);
	glUniform1i(textureSamplerID, 0);

	//Pass the ModelViewProjection matrix to the shaders.
	glUniformMatrix4fv(modelViewProjectionMatrixID, 1, GL_FALSE, &pipelineMatrix[0][0]);

	//Enable the vertex buffer. Specifies the location and data format of the vertex buffer.
	glEnableVertexAttribArray(vertexPosInModelSpaceID);
	glBindBuffer(GL_ARRAY_BUFFER, myFloor->getVertexBufferID());
	glVertexAttribPointer(vertexPosInModelSpaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Enable the color buffer. Specifies the location and data format of the color buffer.
	glEnableVertexAttribArray(vertexColorID);
	glBindBuffer(GL_ARRAY_BUFFER, myFloor->getColorBufferID());
	glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, myFloor->getUVBufferID());
	glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myFloor->getIndexBufferID());
	glDrawElements(GL_TRIANGLES, sizeof(myFloor->floor_indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	//Disable the use of vertex and color buffers;
	glDisableVertexAttribArray(vertexPosInModelSpaceID);
	glDisableVertexAttribArray(vertexColorID);
	glDisableVertexAttribArray(vertexUVID);
}

//Creates, compiles and loads the shader data. Returns a programID for the compiled shader(s).
GLuint ShaderManager::loadShaders(GLFWwindow* window, const char* vertexFilePath, const char* fragmentFilePath)
{
	const char* vertexShaderCode;
	const char* fragmentShaderCode;
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	Utils &utility = Utils::getInstance();

	//Create the shaders.
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read the Vertex Shader code from the file.
	printf("Reading shader: %s\n\0", vertexFilePath);
	vertexShaderCode = utility.readFile(window, vertexFilePath);

	//Compile the Vertex Shader
	printf("Compiling shader : %s\n\0", vertexFilePath);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);

	//Check to see if the Vertex Shader compiled successfully.
	checkShaderStatus(window, vertexShaderID, GL_COMPILE_STATUS);

	//Read the Fragment Shader code from the file.
   	printf("Reading shader: %s\n\0", fragmentFilePath);
	fragmentShaderCode = utility.readFile(window, fragmentFilePath);

	//Compile Fragment Shader.
	printf("Compiling shader : %s\n\0", fragmentFilePath);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShaderID);

	//Check to see if the Fragment Shader compiled successfully.
	checkShaderStatus(window, fragmentShaderID, GL_COMPILE_STATUS);

	//Create the program.
	printf("Creating shader program.\n\0");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//Create the program.
	printf("Linking shader program.\n\0");
	glLinkProgram(programID);

	//Check the Linking status.
	checkShaderStatus(window, programID, GL_LINK_STATUS);

	//Tell openGL to use our compiled shaders.
	glUseProgram(programID);

	//Load specific scaling, rotation, and position matrix transforms for the floor and cube.
	loadModelAndCameraTransforms();

	//Remove the shaders since they are now compiled and linked.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

void ShaderManager::loadModelAndCameraTransforms()
{
	//Set the background color of the window.
	myCamera->setBackgroundColor(vec3(0.0, 0.0, 0.0));

	//Set transforms for the camera.
	myCamera->setMoveSpeed(0.05f);
	myCamera->setFieldOfView(45.0f);
	myCamera->setAspectRatio(4.0f / 3.0f);
	myCamera->setNearClipPlane(0.1f);
	myCamera->setFarClipPlane(1000.0f);
	myCamera->setCameraPos(vec3(0, 3, 6));
	myCamera->setCameraLookAtPoint(vec3(0, 0, 0));
	myCamera->setCameraUpAxis(vec3(0, 1, 0));

	//Set transforms for the cube.
	myCube->setPosition(vec3(0, 0, 0));
	myCube->setRotationAxis(vec3(1, 1, 0));
	myCube->setScale(vec3(1, 1, 1));

	//Set transforms for the floor.
	myFloor->setPosition(vec3(0, -9, 0));
	myFloor->setRotationAxis(vec3(1, 0, 0));
	myFloor->setScale(vec3(20, 1, 20));
}

void ShaderManager::drawShaders(GLFWwindow* window, GLuint programID, GLuint floorTextureID)
{
	static float currAngle = 0.0f;
	mat4 modelViewProjectionMatrix;
	mat4 cubeModelMatrix = mat4(1.0f);
	mat4 floorModelMatrix = mat4(1.0f);
	mat4 worldMatrix = mat4(1.0f);

	//Clear the buffer and prepare it for drawing.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Increment currAngle baised off of clock cycles, once currAngle > 360 then currAngle resets to 0.
	currAngle = fmod((currAngle + 0.0001f), 360.0f);

	cubeModelMatrix = rotate(cubeModelMatrix, currAngle, myCube->getRotationAxis());
	floorModelMatrix = scale(floorModelMatrix, myFloor->getScale()) * translate(floorModelMatrix, myFloor->getPosition()) * rotate(floorModelMatrix, -90.0f, myFloor->getRotationAxis());

	//Calculate our perspective projection and camera position matrtix and send that to the shader.
	modelViewProjectionMatrix = myCamera->calculateMVPMatrix(worldMatrix);
	
	//Calculate the positions of each object with respect to perspective, camera and the world, Then send that data to the shaders.
	sendCubeDataToShaders(window, programID, modelViewProjectionMatrix, cubeModelMatrix);
	sendFloorDataToShaders(window, programID, floorTextureID, modelViewProjectionMatrix, floorModelMatrix);
}
