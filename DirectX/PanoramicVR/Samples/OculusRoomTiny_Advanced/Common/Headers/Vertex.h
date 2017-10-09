/************************************************************************************
Filename		:   Vertex.h
Content			:	Class responsible for holding both vertex data and texture coordinate data.
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

//Contains Position, Color, and Texture coordinates (U,V).
class Vertex : public Object
{
protected:
	XMFLOAT3 myPosition;
	DWORD myColor;
	float myU;
	float myV;

public:
	///Constructors
	Vertex();
	Vertex(const Vertex& vertex);
	Vertex(XMFLOAT3 position);
	Vertex(float x, float y, float z);
	Vertex(XMFLOAT3 position, DWORD color, float u, float v);
	Vertex(float x, float y, float z, DWORD color, float u, float v);

	///Functions
	float CalculateVectorLength();
	static float CalculateVectorLength(Vertex vector);
	static float CalculateVectorLength(XMFLOAT3 vector);
	static float CalculateVectorLength(float x, float y, float z);
	void Normalize();
	static Vertex Normalize(Vertex vectorToNormalize);
	static XMFLOAT3 Normalize(XMFLOAT3 vectorToNormalize);

	///Getters
	XMFLOAT3 GetPosition();
	float GetX();
	float GetY();
	float GetZ();
	DWORD GetColor();
	float GetU();
	float GetV();

	///Setters
	void SetPosition(XMFLOAT3 position);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetColor(DWORD color);
	void SetU(float u);
	void SetV(float v);
};