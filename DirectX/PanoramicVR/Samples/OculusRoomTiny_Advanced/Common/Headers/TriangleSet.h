/************************************************************************************
Filename		:   TriangleSet.h
Content			:	Class responsible for containing vertex and index data.
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
#pragma once
#include "../Headers/stdafx.h"

static const int MAX_TRIANGLES = 50000;

class TriangleSet : public Object
{
protected:
	int myNumIndices;
	int myNumVertices;
	int myMaxBufferSize;
	short* myIndices;
	Vertex* myVertices;

public:
	///Constructor
	TriangleSet(int maxTriangles = MAX_TRIANGLES, short* indices = NULL, Vertex* vertices = NULL);

	///Destructor
	~TriangleSet();

	///Functions
	void AddQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3);
	void AddTriangle(Vertex v0, Vertex v1, Vertex v2, bool generateIndices = true);
	void CreatePanoramic(int angleOfPanoramicImage, float radius, float height);
	void CreateSphere(float radius);
	void LoadGeomerty(vector<VertexPositionNormalTexture> vertices, vector<uint16_t> indices);

	///Getters
	short* GetIndices();
	int GetMaxBufferSize();
	int GetNumIndices();
	int GetNumVertices();
	Vertex* GetVertices();
};
