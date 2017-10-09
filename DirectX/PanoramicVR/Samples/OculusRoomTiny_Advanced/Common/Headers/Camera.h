/************************************************************************************
Filename		:   Camera.h
Content			:	Class responsible for handling scene viewing with respect to camera orientation.
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

class Camera : public Object
{
protected:
	XMVECTOR myPosition;
	XMVECTOR myRotation;

public:
	///Constructors
	Camera();
	Camera(XMVECTOR* position, XMVECTOR* rotation);
	void Init(XMVECTOR position, XMVECTOR rotation);

	///Destructor
	virtual ~Camera();

	///Getters
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMMATRIX GetViewMatrix();

	///Setters
	void SetPosition(XMVECTOR pos);
	void SetRotation(XMVECTOR rot);
};