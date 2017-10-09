/************************************************************************************
Filename		:   OculusTexture.h
Content			:	Class containing texture data specific to OVR.
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
#include "stdafx.h"

static const int TEXTURE_COUNT = 2;

class OculusTexture : public Object
{
protected:
	ovrHmd myHMD;
	ovrSwapTextureSet* mySwapTextureSet;
	ID3D11RenderTargetView* myRenderTargets[TEXTURE_COUNT];
	int mySizeWidth;
	int mySizeHeight;

public:
	///Constructors
	OculusTexture();
	bool Init(ovrHmd hmd, int sizeWidth, int sizeHeight);

	///Destructor
	~OculusTexture();
	
	///Functions
	void Increment();

	///Getters
	ovrHmd GetHMD();
	ID3D11RenderTargetView* GetRenderTarget(int index);
	int GetSizeHeight();
	int GetSizeWidth();
	ovrSwapTextureSet* GetSwapTextureSet();
};