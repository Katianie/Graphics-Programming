/************************************************************************************
Filename		:   BasicVR.h
Content			:	Class that handles the main interaction of OVR, DirectX, and the Scene.
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
#include "../Headers/VRLayer.h"
#include "../Headers/Camera.h"
#include "../Headers/Scene.h"

static const int MAX_LAYERS = 32;

class BasicVR : public Object
{
protected:
	HINSTANCE myHInstance;
	LPCWSTR myWindowTitle;
	ovrHmd myHMD;
	ovrHmdDesc myHMDDescription;
	ovrResult myCurrentResult;
	ovrTexture* myMirrorTexture;
	VRLayer* myVRLayers[MAX_LAYERS];
	Camera* myMainCamera;
	Scene* myRoomScene;
	Keyboard* myKeyboard;
	Keyboard::KeyboardStateTracker* myKeyboardStateTracker;
	bool myIsInitialized;
	bool myIsWindowed;
	float myWindowWidthScale;
	float myWindowHeightScale;
	float myMirrorWidthScale;
	float myMirrorHeightScale;
	char* myImageFilePath;
	char* myTextureToolPath;
	char* myTextureOutputPath;
	char* myConfigFilePath;
	char* myVertexShaderPath;
	char* myPixelShaderPath;

public:
	//Implemented in subclasses (in this case main.h)
	virtual void MainLoop() = 0;

	///Constructor
	BasicVR(HINSTANCE hInst, LPCWSTR windowTitle = L"PanoramicVR\0");
	bool Init();

	///Destructor
	virtual ~BasicVR();

	///Functions
	void ActionFromInput();
	void DistortAndPresent(int numLayersToRender);
	bool HandleMessages();
	void ReleaseDevice();
	int Run();
};