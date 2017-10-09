/************************************************************************************
Filename		:   AceSize.cpp
Content			:	Implementation of the AceSize class.
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
#include "../Headers/AceSize.h"

/**
 * Default Constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 */
AceSize::AceSize()
{
	myWidth = 0;
	myHeight = 0;
}

/**
 * Copy constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param size The size.
 */
AceSize::AceSize(const AceSize& size)
{
	myWidth = size.myWidth;
	myHeight = size.myHeight;
}

/**
 * Parameterized Constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param size The size.
 */
AceSize::AceSize(XMINT2 size)
{
	myWidth = size.x;
	myHeight = size.y;
}

/**
 * Parameterized Constructor.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param width  The width.
 * @param height The height.
 */
AceSize::AceSize(int width, int height)
{
	myWidth = width;
	myHeight = height;
}

/**
 * Calculates the area of the rectangle.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The calculated area of the rectangle.
 */
int AceSize::CalculateArea()
{
	return AceSize::CalculateArea(myWidth, myHeight);
}

/**
 * Calculates the area using the given width and height.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param width  The width.
 * @param height The height.
 *
 * @return The calculated area.
 */
int AceSize::CalculateArea(int width, int height)
{
	return width * height;
}

/**
 * Calculates the perimeter of the rectangle.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The calculated perimeter of the rectangle.
 */
int AceSize::CalculatePerimeter()
{
	return AceSize::CalculatePerimeter(myWidth, myHeight);
}

/**
 * Calculates the perimeter using the given width and height.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param width  The width.
 * @param height The height.
 *
 * @return The calculated perimeter.
 */
int AceSize::CalculatePerimeter(int width, int height)
{
	return 2 * (width + height);
}

/**
 * Gets the width.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The width.
 */
int AceSize::GetWidth()
{
	return myWidth;
}

/**
 * Gets the height.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @return The height.
 */
int AceSize::GetHeight()
{
	return myHeight;
}

/**
 * Sets the width.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param width The width.
 */
void AceSize::SetWidth(int width)
{
	myWidth = width;
}

/**
 * Sets the height.
 *
 * @author Katianie
 * @date 7/30/2016
 *
 * @param height The height.
 */
void AceSize::SetHeight(int height)
{
	myHeight = height;
}
