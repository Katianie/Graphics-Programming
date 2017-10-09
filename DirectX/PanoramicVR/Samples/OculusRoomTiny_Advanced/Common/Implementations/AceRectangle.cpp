/************************************************************************************
Filename		:   AceRectangle.cpp
Content			:	Implementation of the AceRectangle class.
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
#include "../Headers/AceRectangle.h"

/**
 * Default constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 */
AceRectangle::AceRectangle()
{
	myPosition = Vertex();
	mySize = Size();
}

/**
 * Copy constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param rectangle The rectangle.
 */
AceRectangle::AceRectangle(const AceRectangle& rectangle)
{
	myPosition = rectangle.myPosition;
	mySize = rectangle.mySize;
}

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param position The position.
 * @param size	   The size.
 */
AceRectangle::AceRectangle(Vertex position, AceSize size)
{
	myPosition = position;
	mySize = size;
}

/**
 * Gets the position.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The position.
 */
Vertex AceRectangle::GetPosition()
{
	return myPosition;
}

/**
 * Gets the size.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The size.
 */
AceSize AceRectangle::GetSize()
{
	return mySize;
}

/**
 * Sets a position.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param position The position.
 */
void AceRectangle::SetPosition(Vertex position)
{
	myPosition = position;
}

/**
 * Sets a size.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param size The size.
 */
void AceRectangle::SetSize(AceSize size)
{
	mySize = size;
}
