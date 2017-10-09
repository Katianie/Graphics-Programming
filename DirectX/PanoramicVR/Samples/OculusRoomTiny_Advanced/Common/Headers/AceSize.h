/************************************************************************************
Filename		:   AceSize.h
Content			:	Class responsible for holding width and height dimensions.
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

//Contains Width and Height of something/anything.
class AceSize : public Object
{
public:
	int myWidth;
	int myHeight;

	///Constructors
	AceSize();
	AceSize(const AceSize& size);
	AceSize(XMINT2 size);
	AceSize(int width, int height);

	///Functions
	int CalculateArea();
	static int CalculateArea(int width, int height);
	int CalculatePerimeter();
	static int CalculatePerimeter(int width, int height);

	///Getters
	int GetWidth();
	int GetHeight();

	///Setters
	void SetWidth(int width);
	void SetHeight(int height);
};