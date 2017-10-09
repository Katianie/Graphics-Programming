/************************************************************************************
Filename		:   Model.h
Content			:	Class responsible for tyeing together the geometry and the material/texture.
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
#include "../Headers/Material.h"
#include "../Headers/TriangleSet.h"

class Model : public Object
{
protected:
	XMFLOAT3 myPosition;
	XMFLOAT4 myFirstPosition;
	XMFLOAT4 myRotation;
	Material* myMaterial;
	DataBuffer* myVertexBuffer;
	DataBuffer* myIndexBuffer;
	int myNumIndices;

public:
	///Constructors
	Model();
	Model(int panoramicType, TriangleSet* triangleSet, XMFLOAT3 modelPosition, XMFLOAT4 modelRotation, Material* modelMaterial);
	void Init(int panoramicType, TriangleSet* triangleSet);

	///Destructor
	~Model();

	///Functions
	void Render(XMMATRIX* projView, float red, float green, float blue, float alpha);

	///Getters
	DataBuffer* GetIndexBuffer();
	Material* GetMaterial();
	int GetNumIndices();
	XMFLOAT3 GetPosition();
	XMFLOAT4 GetRotation();
	DataBuffer* GetVertexBuffer();
	
	///Setters
	void SetPosition(XMFLOAT3 pos);
};