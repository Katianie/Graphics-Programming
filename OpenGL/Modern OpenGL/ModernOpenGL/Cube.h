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

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

class Cube
{
private:
	GLuint myVertexBufferID;
	GLuint myIndexBufferID;
	GLuint myColorBufferID;
	vec3 myScale;
	vec3 myRotationAxis;
	vec3 myPosition;

	void generateAndBindBuffers();

public:
	GLfloat cube_vertices[24];
	GLfloat cube_colors[24];
	GLushort cube_indices[36];

	Cube();
	virtual ~Cube()
	{

	}

	//Getters
	GLuint getVertexBufferID();
	GLuint getIndexBufferID();
	GLuint getColorBufferID();
	vec3 getScale();
	vec3 getRotationAxis();
	vec3 getPosition();

	//Setters
	void setScale(vec3 scale);
	void setRotationAxis(vec3 axis);
	void setPosition(vec3 pos);

};