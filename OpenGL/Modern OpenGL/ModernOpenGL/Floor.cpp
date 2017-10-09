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
#include "Floor.h"

Floor::Floor()
{
	//Load default vertices.
	floor_vertices[0] = -1.0f; floor_vertices[1] = -1.0f; floor_vertices[2] = 1.0f;
	floor_vertices[3] = 1.0f;  floor_vertices[4] = -1.0f; floor_vertices[5] = 1.0f;
	floor_vertices[6] = 1.0f;  floor_vertices[7] = 1.0f;  floor_vertices[8] = 1.0f;
	floor_vertices[9] = -1.0f; floor_vertices[10] = 1.0f; floor_vertices[11] = 1.0f;

	//Load default UV coordinates.
	floor_UVs[0] = -4.0f;  floor_UVs[1] = -4.0f;
	floor_UVs[2] = 4.0f;   floor_UVs[3] = -4.0f;
	floor_UVs[4] = 4.0f;   floor_UVs[5] = 4.0f;
	floor_UVs[6] = -4.0f;  floor_UVs[7] = 4.0f;

	//Load default colors.
	floor_colors[0] = 0.0f;  floor_colors[1] = 0.0f;  floor_colors[2] = 0.0f;
	floor_colors[3] = 0.0f;  floor_colors[4] = 0.0f;  floor_colors[5] = 0.0f;
	floor_colors[6] = 0.0f;  floor_colors[7] = 0.0f;  floor_colors[8] = 0.0f;
	floor_colors[9] = 0.0f;  floor_colors[10] = 0.0f; floor_colors[11] = 0.0f;

	//Load default indices.
	floor_indices[0] = 0;	floor_indices[1] = 1;	floor_indices[2] = 2;
	floor_indices[3] = 2;	floor_indices[4] = 3;	floor_indices[5] = 0;

	//Create the VBO for the floor and pass the data to the buffers.
	generateAndBindBuffers();
}

//Getters
GLuint Floor::getVertexBufferID()
{
	return myVertexBufferID;
}

GLuint Floor::getIndexBufferID()
{
	return myIndexBufferID;
}

GLuint Floor::getColorBufferID()
{
	return myColorBufferID;
}

GLuint Floor::getUVBufferID()
{
	return myUVBufferID;
}

vec3 Floor::getScale()
{
	return myScale;
}

vec3 Floor::getRotationAxis()
{
	return myRotationAxis;
}

vec3 Floor::getPosition()
{
	return myPosition;
}

//Setters
void Floor::setScale(vec3 scale)
{
	myScale = scale;
}

void Floor::setRotationAxis(vec3 axis)
{
	myRotationAxis = axis;
}

void Floor::setPosition(vec3 pos)
{
	myPosition = pos;
}

void Floor::generateAndBindBuffers()
{
	glGenBuffers(1, &myVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &myColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_colors), floor_colors, GL_STATIC_DRAW);		//Give color data to Shaders.

	glGenBuffers(1, &myUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_UVs), floor_UVs, GL_STATIC_DRAW);

	//Generate a buffer for the indices as well
	glGenBuffers(1, &myIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}