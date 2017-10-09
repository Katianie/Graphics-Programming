/************************************************************************************
Filename		:   Vertex.cpp
Content			:	Implementation of the Vertex class.
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
#include "../Headers/Vertex.h"

/**
 * Default Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Vertex::Vertex()
{
	myPosition = XMFLOAT3();
	myColor = 0xffffffff;
	myU = 0.0f;
	myV = 0.0f;
}

/**
 * Copy constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vertex The vertex.
 */
Vertex::Vertex(const Vertex& vertex)
{
	myPosition = vertex.myPosition;
	myColor = vertex.myColor;
	myU = vertex.myU;
	myV = vertex.myV;
}

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param position The position.
 */
Vertex::Vertex(XMFLOAT3 position)
{
	myPosition = position;
	myColor = 0xffffffff;
	myU = 0.0f;
	myV = 0.0f;
}

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param x The x position coordinate.
 * @param y The y position coordinate.
 * @param z The z position coordinate.
 */
Vertex::Vertex(float x, float y, float z)
{
	myPosition = XMFLOAT3(x, y, z);
	myColor = 0xffffffff;
	myU = 0.0f;
	myV = 0.0f;
}

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param position The position.
 * @param color    The color.
 * @param u		   The u texture coordinate.
 * @param v		   The v texture coordinate.
 */
Vertex::Vertex(XMFLOAT3 position, DWORD color, float u, float v)
{
	myPosition = position;
	myColor = color;
	myU = u;
	myV = v;
}

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param x	    The x coordinate.
 * @param y	    The y coordinate.
 * @param z	    The z coordinate.
 * @param color The color.
 * @param u		The u texture coordinate.
 * @param v		The v texture coordinate.
 */
Vertex::Vertex(float x, float y, float z, DWORD color, float u, float v)
{
	myPosition = XMFLOAT3(x, y, z);
	myColor = color;
	myU = u;
	myV = v;
}

/**
 * Calculate the length of the vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The calculated vector length.
 */
float Vertex::CalculateVectorLength()
{
	return sqrt((Vertex::GetX() * Vertex::GetX()) +
				(Vertex::GetY() * Vertex::GetY()) +
				(Vertex::GetZ() * Vertex::GetZ()));
}

/**
 * Calculates the length of the vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vector The vector.
 *
 * @return The calculated vector length.
 */
float Vertex::CalculateVectorLength(Vertex vector)
{
	return sqrt((vector.GetX() * vector.GetX()) +
				(vector.GetY() * vector.GetY()) +
				(vector.GetZ() * vector.GetZ()));
}

/**
 * Calculates the length of the vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vector The vector.
 *
 * @return The calculated vector length.
 */
float Vertex::CalculateVectorLength(XMFLOAT3 vector)
{
	return sqrt((vector.x * vector.x) +
				(vector.y * vector.y) +
				(vector.z * vector.z));
}

/**
 * Calculates the length of the vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param z The z coordinate.
 *
 * @return The calculated vector length.
 */
float Vertex::CalculateVectorLength(float x, float y, float z)
{
	return sqrt((x * x) +
				(y * y) +
				(z * z));
}

/**
 * Normalizes myPosition.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void Vertex::Normalize()
{
	float vectLen = this->CalculateVectorLength();

	myPosition.x = myPosition.x / vectLen;
	myPosition.y = myPosition.y / vectLen;
	myPosition.z = myPosition.z / vectLen;
}

/**
 * Normalizes the given vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vectorToNormalize The vector to normalize.
 *
 * @return A vertex containing a normalized position.
 */
Vertex Vertex::Normalize(Vertex vectorToNormalize)
{
	float vectLen = vectorToNormalize.CalculateVectorLength();

	vectorToNormalize.SetX(vectorToNormalize.GetX() / vectLen);
	vectorToNormalize.SetY(vectorToNormalize.GetY() / vectLen);
	vectorToNormalize.SetZ(vectorToNormalize.GetZ() / vectLen);

	return vectorToNormalize;
}

/**
 * Normalizes the given vector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vectorToNormalize The vector to normalize.
 *
 * @return An XMFLOAT3.
 */
XMFLOAT3 Vertex::Normalize(XMFLOAT3 vectorToNormalize)
{
	Vertex retVal(vectorToNormalize.x, vectorToNormalize.y, vectorToNormalize.z);
	retVal.Normalize();

	return XMFLOAT3(retVal.GetX(), retVal.GetY(), retVal.GetZ());
}

/**
 * Gets the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The position.
 */
XMFLOAT3 Vertex::GetPosition()
{
	return myPosition;
}

/**
 * Get x coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The x coordinate of the position.
 */
float Vertex::GetX()
{
	return myPosition.x;
}

/**
 * Get y coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The y coordinate of the position.
 */
float Vertex::GetY()
{
	return myPosition.y;
}

/**
 * Get z coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The z coordinate of the position.
 */
float Vertex::GetZ()
{
	return myPosition.z;
}

/**
 * Gets the color.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The color.
 */
DWORD Vertex::GetColor()
{
	return myColor;
}

/**
 * Gets the u Texture coordinate.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The u Texture coordinate.
 */
float Vertex::GetU()
{
	return myU;
}

/**
 * Gets the v Texture coordinate.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The v Texture coordinate.
 */
float Vertex::GetV()
{
	return myV;
}

/**
 * Sets the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param position The position.
 */
void Vertex::SetPosition(XMFLOAT3 position)
{
	myPosition = position;
}

/**
 * Sets the x coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param x The x coordinate of the position.
 */
void Vertex::SetX(float x)
{
	myPosition.x = x;
}

/**
 * Sets a y coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param y The y coordinate of the position.
 */
void Vertex::SetY(float y)
{
	myPosition.y = y;
}

/**
 * Sets a z coordinate of the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param z The z coordinate of the position. 
 */
void Vertex::SetZ(float z)
{
	myPosition.z = z;
}

/**
 * Sets a color.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param color The color.
 */
void Vertex::SetColor(DWORD color)
{
	myColor = color;
}

/**
 * Sets a u Texture coordinate.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param u The u Texture coordinate.
 */
void Vertex::SetU(float u)
{
	myU = u;
}

/**
 * Sets a v Texture coordinate.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param v The v Texture coordinate.
 */
void Vertex::SetV(float v)
{
	myV = v;
}
