/************************************************************************************
Filename		:   VRLayer.h
Content			:	Class that handles display of scene to the HMD.
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
#include "../Headers/OculusTexture.h"
#include "../Headers/Camera.h"
#include "../Headers/Scene.h"

class VRLayer : public Object
{
protected:
	DepthBuffer* myEyeDepthBuffers[2];					//For the eye buffers to use when rendered.
	ovrEyeRenderDesc myEyeRenderDescriptions[2];        //Description of the VR, Contains info for FOV.
	ovrPosef myEyeRenderPoses[2];						//Rotation and Position together is a Pose.
	OculusTexture* myEyeRenderTextures[2];				//Where the eye buffers will be rendered.
	ovrRecti myEyeRenderViewports[2];					//Useful to remember when varying resolution.
	ovrHmd myHMD;										//The Head Mounted Display (i.e. The Oculus Rift)
	ovrLayerEyeFov myOvrLayer;							//Contains info for stereoscopic rendering.

public:
	///Constructor
	VRLayer(ovrHmd hmd, const ovrFovPort* fovs = NULL, float pixelsPerDisplayPixel = 1.0f);

	///Destructor
	virtual ~VRLayer();
	
	///Functions
	ovrTrackingState CalculateEyePoses();
	void InitializeLayerHeader();
	void MakeEyeBuffers(float pixelsPerDisplayPixel = 1.0f);
	void ModifyFOVS(const ovrFovPort* fovs = NULL);
	XMMATRIX RenderSceneToEyeBuffer(Camera* playerCamera, Scene* sceneToRender, int eyeIndex, ID3D11RenderTargetView* renderTarget = NULL, ovrPosef* eyeRenderPose = NULL, int timesToRenderRoom = 1, float alpha = 1.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float nearZ = 0.2f, float farZ = 1000.0f, bool doWeSetupRender = true, DepthBuffer* depthBuffer = NULL, float backRed = 0.0f, float backGreen = 0.0f, float backBlue = 0.0f);
	
	///Getters
	DepthBuffer** GetEyeDepthBuffers();
	ovrEyeRenderDesc* GetEyeRenderDescriptions();
	ovrPosef* GetEyeRenderPoses();
	OculusTexture** GetEyeRenderTextures();
	ovrRecti* GetEyeRenderViewports();
	ovrHmd GetHMD();
	ovrLayerEyeFov& GetOvrLayer();
};