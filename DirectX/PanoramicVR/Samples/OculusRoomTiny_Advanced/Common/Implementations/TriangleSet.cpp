/************************************************************************************
Filename		:   TriangleSet.cpp
Content			:	Implementation of the TriangleSet class.
Created			:   October 20th, 2014
Modified		:	July 4th, 2016
Original Author :   Tom Heath
Modified By		:	Eddie O'Hagan (Katianie)
Copyright		:   Copyright 2014 Oculus, Inc and Katianie.
All Rights reserved. Licensed under the Apache License, Version 2.0
(the "License"); you may not use this file except in compliance
with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/
#include "../Headers/stdafx.h"
#include "../Headers/TriangleSet.h"

/**
 * Constructor, allocates a vertex and index buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param maxTriangles	    The maximum number of triangles to allow.
 * @param [in,out] indices  An index buffer, if null, its created here.
 * @param [in,out] vertices A vertex buffer, if null, its created here.
 */
TriangleSet::TriangleSet(int maxTriangles, short* indices, Vertex* vertices)
{
	myNumIndices = 0;
	myNumVertices = 0;
	myMaxBufferSize = 3 * maxTriangles;

	//Check to see if a index array was provided.
	if (indices != NULL)
	{
		myIndices = indices;
	}
	else
	{
		myIndices = (short*)_aligned_malloc(myMaxBufferSize * sizeof(short), BYTE_ALIGNMENT);
	}

	//Check to see if a vertex array was provided.
	if (vertices != NULL)
	{
		myVertices = vertices;
	}
	else
	{
		myVertices = (Vertex*)_aligned_malloc(myMaxBufferSize * sizeof(Vertex), BYTE_ALIGNMENT);
	}
}

/**
 * Destructor, deletes and cleans up index and vertex buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 */
TriangleSet::~TriangleSet()
{
	_aligned_free(myIndices);
	_aligned_free(myVertices);
}

/**
 * Creates a square using triangles.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param v0 The first vertex of the square.
 * @param v1 The second vertex of the square.
 * @param v2 The third vertex of the square.
 * @param v3 The fourth vertex of the square.
 */
void TriangleSet::AddQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
{
	TriangleSet::AddTriangle(v0, v1, v2);
	TriangleSet::AddTriangle(v3, v2, v1);
}

/**
 * Adds a single triangle to the vertex and index buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param v0			  The first vertex.
 * @param v1			  The second Vertex.
 * @param v2			  The third Vertex.
 * @param generateIndices true to generate indices.
 */
void TriangleSet::AddTriangle(Vertex v0, Vertex v1, Vertex v2, bool generateIndices)
{
	AceDX::Validate(myNumVertices <= (myMaxBufferSize - 3), "Number of verties is past the limit.");

	if (generateIndices == true)
	{
		//Generate the indices.
		for (int i = 0; i < 3; i++)
		{
			myIndices[myNumIndices++] = myNumVertices + i;
		}
	}
	
	myVertices[myNumVertices++] = v0;
	myVertices[myNumVertices++] = v1;
	myVertices[myNumVertices++] = v2;
}

/**
 * Creates the partial cylinder geometry used for 180, 270, and 360 views.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param angleOfPanoramicImage The angle of panoramic image (180, 270, or 360).
 * @param radius			    The radius of the partial cylinder.
 * @param height			    The height of the partial cylinder.
 */
void TriangleSet::CreatePanoramic(int angleOfPanoramicImage, float radius, float height)
{
	vector<VertexPositionNormalTexture> vertices;
	vector<uint16_t> indices;
	vector<Vertex> positions;
	unsigned int tessellation = 32U;
	float angleOFCylinder;
	float angle;
	float dx;
	float dz;
	float u;
	XMVECTOR textureCoordinate = XMLoadFloat(&u);
	XMVECTOR topOffset = g_XMIdentityR1 * height;
	XMVECTOR normal;
	XMVECTOR sideOffset;
	int stride = tessellation + 1;

	//Copied from the GeometricPrimitive class for creating a cylinder, 
	//In this case however, we leave out the top and bottom parts.
	for (unsigned int i = 0; i <= tessellation; i++)
	{
		//Calculate the CirlceVector.
		angleOFCylinder = (float)Ace::DegreesToRadians((double)angleOfPanoramicImage);
		angle = i * angleOFCylinder / tessellation;
		XMScalarSinCos(&dx, &dz, angle);
		normal = { dx, 0, dz, 0 };

		sideOffset = normal * radius;

		//Calculate texture coordinate for the partial cylinder.
		u = (float)i / tessellation;
		textureCoordinate = XMLoadFloat(&u);

		vertices.push_back(VertexPositionNormalTexture(sideOffset + topOffset, normal, textureCoordinate));
		vertices.push_back(VertexPositionNormalTexture(sideOffset - topOffset, normal, textureCoordinate + g_XMIdentityR1));

		indices.push_back(i * 2);
		indices.push_back((i * 2 + 2) % (stride * 2));
		indices.push_back(i * 2 + 1);

		indices.push_back(i * 2 + 1);
		indices.push_back((i * 2 + 2) % (stride * 2));
		indices.push_back((i * 2 + 3) % (stride * 2));
	}

	this->LoadGeomerty(vertices, indices);
}

/**
 * Creates the sphere used for photosphere views.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param radius The radius of the sphere.
 */
void TriangleSet::CreateSphere(float radius)
{
	vector<VertexPositionNormalTexture> vertices;
	vector<uint16_t> indices;
	vector<Vertex> positions;
	size_t tessellation = 32U;

	GeometricPrimitive::CreateSphere(vertices, indices, radius * 2, tessellation, true, true);

	this->LoadGeomerty(vertices, indices);
}

/**
 * Takes in raw vertex and index buffers and creates 
 * the triangles of the model and assembles them.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vertices The vertices to load.
 * @param indices  The indices to load.
 */
void TriangleSet::LoadGeomerty(vector<VertexPositionNormalTexture> vertices, vector<uint16_t> indices)
{
	float x;
	float y;
	float z;
	Vertex currVertex;
	vector<Vertex> positions;

	for (int i = 0; i < myNumIndices; i++)
	{
		myIndices[i] = 0;
	}

	myNumIndices = 0;
	for (size_t i = 0; i < indices.size(); i++)
	{
		myIndices[myNumIndices++] = indices[i];
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		x = vertices[i].position.x;
		y = vertices[i].position.y;
		z = vertices[i].position.z;
		currVertex = Vertex(x, y, z);

		//Invert the U texture coordinate since we are painting the inside of the sphere.
		currVertex.SetU(vertices[i].textureCoordinate.x * -1);
		currVertex.SetV(vertices[i].textureCoordinate.y);

		positions.push_back(currVertex);
	}

	for (size_t i = 0; i < positions.size() - 2; i += 3)
	{
		this->AddTriangle(positions[i], positions[i + 1], positions[i + 2], false);
	}
}

/**
 * Gets the index buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The index buffer.
 */
short* TriangleSet::GetIndices()
{
	return myIndices;
}

/**
 * Gets maximum buffer size.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The maximum buffer size.
 */
int TriangleSet::GetMaxBufferSize()
{
	return myMaxBufferSize;
}

/**
 * Gets number indices.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The number indices.
 */
int TriangleSet::GetNumIndices()
{
	return myNumIndices;
}

/**
 * Gets number vertices.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The number vertices.
 */
int TriangleSet::GetNumVertices()
{
	return myNumVertices;
}

/**
 * Gets the vertex buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The vertex buffer.
 */
Vertex* TriangleSet::GetVertices()
{
	return myVertices;
}