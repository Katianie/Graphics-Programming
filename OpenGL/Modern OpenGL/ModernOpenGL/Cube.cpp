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
#include "Cube.h"

Cube::Cube()
{
	//Load default vertices for the front of the cube.
	cube_vertices[0] = -1.0f;	cube_vertices[1] = -1.0f;	cube_vertices[2] = 1.0f;
	cube_vertices[3] = 1.0f;	cube_vertices[4] = -1.0f;	cube_vertices[5] = 1.0f;
	cube_vertices[6] = 1.0f;	cube_vertices[7] = 1.0f;	cube_vertices[8] = 1.0f;
	cube_vertices[9] = -1.0f;	cube_vertices[10] = 1.0f;	cube_vertices[11] = 1.0f;
	
	//Load default vertices for the back of the cube.
	cube_vertices[12] = -1.0f;	cube_vertices[13] = -1.0f;	cube_vertices[14] = -1.0f;
	cube_vertices[15] = 1.0f;	cube_vertices[16] = -1.0f;	cube_vertices[17] = -1.0f;
	cube_vertices[18] = 1.0f;	cube_vertices[19] = 1.0f;	cube_vertices[20] = -1.0f;
	cube_vertices[21] = -1.0f;	cube_vertices[22] = 1.0f;	cube_vertices[23] = -1.0f;

	//Load default colors for the front of the cube.
	cube_colors[0] = 1.0f;	cube_colors[1] = 0.0f;	cube_colors[2] = 0.0f;
	cube_colors[3] = 0.0f;	cube_colors[4] = 1.0f;	cube_colors[5] = 0.0f;
	cube_colors[6] = 0.0f;	cube_colors[7] = 0.0f;	cube_colors[8] = 1.0f;
	cube_colors[9] = 0.0f;	cube_colors[10] = 1.0f;	cube_colors[11] = 0.0f;
	//Load default colors for the back of the cube.
	cube_colors[12] = 1.0f;	cube_colors[13] = 0.0f;	cube_colors[14] = 0.0f;
	cube_colors[15] = 0.0f;	cube_colors[16] = 1.0f;	cube_colors[17] = 0.0f;
	cube_colors[18] = 0.0f;	cube_colors[19] = 0.0f;	cube_colors[20] = 1.0f;
	cube_colors[21] = 1.0f;	cube_colors[22] = 0.0f;	cube_colors[23] = 0.0f;

	//Load default indices for the front of the cube.
	cube_indices[0] = 0;  cube_indices[1] = 1;  cube_indices[2] = 2;
	cube_indices[3] = 2;  cube_indices[4] = 3;  cube_indices[5] = 0;
	
	//Load default indices for the top of the cube.
	cube_indices[6] = 3;  cube_indices[7] = 2;  cube_indices[8] = 6;
	cube_indices[9] = 6;  cube_indices[10] = 7; cube_indices[11] = 3;
	
	//Load default indices for the back of the cube.
	cube_indices[12] = 7; cube_indices[13] = 6; cube_indices[14] = 5;
	cube_indices[15] = 5; cube_indices[16] = 4; cube_indices[17] = 7;
	
	//Load default indices for the bottom of the cube.
	cube_indices[18] = 4; cube_indices[19] = 5; cube_indices[20] = 1;
	cube_indices[21] = 1; cube_indices[22] = 0; cube_indices[23] = 4;
	
	//Load default indices for the left of the cube.
	cube_indices[24] = 4; cube_indices[25] = 0; cube_indices[26] = 3;
	cube_indices[27] = 3; cube_indices[28] = 7; cube_indices[29] = 4;
	
	//Load default indices for the right of the cube.
	cube_indices[30] = 1; cube_indices[31] = 5; cube_indices[32] = 6;
	cube_indices[33] = 6; cube_indices[34] = 2; cube_indices[35] = 1;

	generateAndBindBuffers();
}

//Getters
GLuint Cube::getVertexBufferID()
{
	return myVertexBufferID;
}

GLuint Cube::getIndexBufferID()
{
	return myIndexBufferID;
}

GLuint Cube::getColorBufferID()
{
	return myColorBufferID;
}

vec3 Cube::getScale()
{
	return myScale;
}

vec3 Cube::getRotationAxis()
{
	return myRotationAxis;
}

vec3 Cube::getPosition()
{
	return myPosition;
}

//Setters
void Cube::setScale(vec3 scale)
{
	myScale = scale;
}

void Cube::setRotationAxis(vec3 axis)
{
	myRotationAxis = axis;
}

void Cube::setPosition(vec3 pos)
{
	myPosition = pos;
}

void Cube::generateAndBindBuffers()
{
	//Obtain handles to shader buffers.
	glGenBuffers(1, &myVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);	//Give vertex data to Shaders.

	glGenBuffers(1, &myColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);		//Give color data to Shaders.

	//Generate a buffer for the indices as well
	glGenBuffers(1, &myIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}