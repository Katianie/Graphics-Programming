/************************************************************************************
Filename		:   VRLayer.cpp
Content			:	Implementation of VRLayer class.
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
#include "../Headers/VRLayer.h"

/**
 * Constructor. Initialize OVR Field of View and other things such
 * as buffer initialization. Each eye has a VR Layer, so for us
 * there are two VRLayers. 
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hmd				    The HMD object.
 * @param fovs				    Custom FOV for the HMD.
 * @param pixelsPerDisplayPixel Default is 1.0, lower is better performance, higher is better quality.
 */
VRLayer::VRLayer(ovrHmd hmd, const ovrFovPort* fovs, float pixelsPerDisplayPixel)
{
	myHMD = hmd;

	MakeEyeBuffers(pixelsPerDisplayPixel);
	ModifyFOVS(fovs);
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
VRLayer::~VRLayer()
{
	for (int eyeIndex = 0; eyeIndex < 2; eyeIndex++)
	{
		delete myEyeRenderTextures[eyeIndex];
		delete myEyeDepthBuffers[eyeIndex];

		myEyeRenderTextures[eyeIndex] = NULL;
		myEyeDepthBuffers[eyeIndex] = NULL;
	}
}

/**
 * Calculates the current eye poses. This needs to be updated 
 * constantly so we know the current position and orientation
 * of the HMD.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return A struct containing the calculated eye poses.
 */
ovrTrackingState VRLayer::CalculateEyePoses()
{
	double predictedDisplayTime;
	ovrTrackingState trackingState;

	//Get both eye poses simultaneously, with IPD offset already included. 
	ovrVector3f useHmdToEyeViewOffset[2] =
	{
		myEyeRenderDescriptions[0].HmdToEyeViewOffset,
		myEyeRenderDescriptions[1].HmdToEyeViewOffset
	};

	predictedDisplayTime = ovr_GetPredictedDisplayTime(myHMD, 0);
	trackingState = ovr_GetTrackingState(myHMD, predictedDisplayTime, true);

	ovr_CalcEyePoses(trackingState.HeadPose.ThePose, useHmdToEyeViewOffset, myEyeRenderPoses);

	return trackingState;
}

/**
 * Initialize layer header. Pass our data over to the OVR layer struct.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void VRLayer::InitializeLayerHeader()
{
	myOvrLayer.Header.Type = ovrLayerType_EyeFov;
	myOvrLayer.Header.Flags = 0;
	myOvrLayer.ColorTexture[0] = myEyeRenderTextures[0]->GetSwapTextureSet();
	myOvrLayer.ColorTexture[1] = myEyeRenderTextures[1]->GetSwapTextureSet();
	myOvrLayer.RenderPose[0] = myEyeRenderPoses[0];
	myOvrLayer.RenderPose[1] = myEyeRenderPoses[1];
	myOvrLayer.Fov[0] = myEyeRenderDescriptions[0].Fov;
	myOvrLayer.Fov[1] = myEyeRenderDescriptions[1].Fov;
	myOvrLayer.Viewport[0] = myEyeRenderViewports[0];
	myOvrLayer.Viewport[1] = myEyeRenderViewports[1];
}

/**
 * Makes the texture and data buffers for the eye render textures.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param pixelsPerDisplayPixel Default is 1.0, lower is better performance, higher is better quality.
 */
void VRLayer::MakeEyeBuffers(float pixelsPerDisplayPixel)
{
	ovrSizei idealTextureSize;

	for (int eyeIndex = 0; eyeIndex < 2; eyeIndex++)
	{
		//eyeIndex = 0 = left
		//eyeIndex = 1 = right
		idealTextureSize = ovr_GetFovTextureSize(myHMD,
												(ovrEyeType)eyeIndex,
												 ovr_GetHmdDesc(myHMD).DefaultEyeFov[eyeIndex],
												 pixelsPerDisplayPixel);

		//Initialize render textures, depth buffers, and viewports.
		myEyeRenderTextures[eyeIndex] = new OculusTexture();
		if (myEyeRenderTextures[eyeIndex]->Init(myHMD, idealTextureSize.w, idealTextureSize.h) == true)
		{
			myEyeDepthBuffers[eyeIndex] = new DepthBuffer(DirectXManager::RetrieveDirectXManager().GetDxDevice(), idealTextureSize.w, idealTextureSize.h);
			myEyeRenderViewports[eyeIndex].Pos.x = 0;
			myEyeRenderViewports[eyeIndex].Pos.y = 0;
			myEyeRenderViewports[eyeIndex].Size = idealTextureSize;
		}
	}
}

/**
 * Modify the FOVs for each eye.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param fovs The custom FOVs for each eye.
 */
void VRLayer::ModifyFOVS(const ovrFovPort* fovs)
{
	if (fovs == NULL)
	{
		//Use the default eye FOV if none is provided.
		fovs = ovr_GetHmdDesc(myHMD).DefaultEyeFov;
	}

	myEyeRenderDescriptions[0] = ovr_GetRenderDesc(myHMD, ovrEye_Left, fovs[0]);
	myEyeRenderDescriptions[1] = ovr_GetRenderDesc(myHMD, ovrEye_Right, fovs[1]);
}

/**
 * Reads in the current HMD pose and data, then renders the scene to both eyes.
 *
 * @author Katianie
 * @date 7/5/2016
 *
 * @param [in] playerCamera		 If non-null, the Player's Camera.
 * @param [in] sceneToRender	 If non-null, the Scene to render.
 * @param eyeIndex				 Zero-based index of the eye.
 * @param [in] renderTarget		 If non-null, the render target.
 * @param [in] eyeRenderPose	 If non-null, the eye render pose.
 * @param timesToRenderRoom		 The number of times to render room.
 * @param alpha					 The alpha.
 * @param red					 The red.
 * @param green					 The green.
 * @param blue					 The blue.
 * @param nearZ					 The near z coordinate.
 * @param farZ					 The far z coordinate.
 * @param doWeSetupRender		 true to do we setup render.
 * @param [in] depthBuffer		 If non-null, buffer for depth data.
 * @param backRed				 The red value of the back buffer clear color.
 * @param backGreen				 The green value of the back buffer clear color.
 * @param backBlue				 The blue value of the back buffer clear color.
 *
 * @return The Model View Projection matrix of the rendered scene.
 */
XMMATRIX VRLayer::RenderSceneToEyeBuffer(Camera* playerCamera, Scene* sceneToRender, int eyeIndex, ID3D11RenderTargetView* renderTarget, ovrPosef* eyeRenderPose, int timesToRenderRoom, float alpha, float red, float green, float blue, float nearZ, float farZ, bool doWeSetupRender, DepthBuffer* depthBuffer, float backRed, float backGreen, float backBlue )
{
	//Get view and projection matrices for the Rift camera
	XMVECTOR cameraPose;
	XMMATRIX viewMatrix;
	ovrMatrix4f projectionMatrix;
	XMMATRIX mvpMatrix;
	Camera finalCamera;
	int textureIndex = -1;

	//If any values are passed as NULL, then we use the default basic case
	if (depthBuffer == NULL)
	{
		depthBuffer = myEyeDepthBuffers[eyeIndex];
	}
	if (eyeRenderPose == NULL)
	{
		eyeRenderPose = &myEyeRenderPoses[eyeIndex];
	}

	if (doWeSetupRender)
	{
		if (renderTarget != NULL)
		{
			DirectXManager::RetrieveDirectXManager().SetAndClearRenderTarget(renderTarget, depthBuffer, backRed, backGreen, backBlue);
		}
		else
		{
			//We increment which texture we are using, to the next one, just before writing
			myEyeRenderTextures[eyeIndex]->Increment();
			textureIndex = myEyeRenderTextures[eyeIndex]->GetSwapTextureSet()->CurrentIndex;

			DirectXManager::RetrieveDirectXManager().SetAndClearRenderTarget(myEyeRenderTextures[eyeIndex]->GetRenderTarget(textureIndex), depthBuffer, backRed, backGreen, backBlue);
		}

		DirectXManager::RetrieveDirectXManager().InitViewport((float)myEyeRenderViewports[eyeIndex].Pos.x,
															  (float)myEyeRenderViewports[eyeIndex].Pos.y,
															  (float)myEyeRenderViewports[eyeIndex].Size.w,
															  (float)myEyeRenderViewports[eyeIndex].Size.h);
	}

	//Get view and projection matrices for the Rift camera
	cameraPose = XMVectorAdd(playerCamera->GetPosition(), XMVector3Rotate(AceDX::ConvertOVRVector3ToXMVector(eyeRenderPose->Position), playerCamera->GetRotation()));
	finalCamera.Init(cameraPose, (XMQuaternionMultiply(AceDX::ConvertOVRQuaternionToXMVector(eyeRenderPose->Orientation), playerCamera->GetRotation())));
	viewMatrix = finalCamera.GetViewMatrix();
	projectionMatrix = ovrMatrix4f_Projection(myEyeRenderDescriptions[eyeIndex].Fov, nearZ, farZ, ovrProjection_RightHanded);
	mvpMatrix = XMMatrixMultiply(viewMatrix, AceDX::ConvertOVRMatrixToXMMatrix(projectionMatrix));

	//Render the scene
	for (int n = 0; n < timesToRenderRoom; n++)
	{
		sceneToRender->Render(&mvpMatrix, red, green, blue, alpha);
	}

	return mvpMatrix;
}

/**
 * Gets the array of Eye Depth buffers which are 
 * responsible for storing Z-Buffer data for each eye.
 *
 * @author Katianie
 * @date 7/5/2016
 *
 * @return The array of Eye Depth buffers.
 */
DepthBuffer** VRLayer::GetEyeDepthBuffers()
{
	return myEyeDepthBuffers;
}

/**
 * Gets the array of Eye Render Descriptions which are 
 * rendering information for each eye.
 *
 * @author Katianie
 * @date 7/5/2016
 *
 * @return The array of Eye Render Descriptions.
 */
ovrEyeRenderDesc* VRLayer::GetEyeRenderDescriptions()
{
	return myEyeRenderDescriptions;
}

/**
 * Gets the array of Eye Render Poses which contain
 * orientation and positioning information about the HMD.
 *
 * @author Katianie
 * @date 7/5/2016
 *
 * @return The array of Eye Render Poses.
 */
ovrPosef* VRLayer::GetEyeRenderPoses()
{
	return myEyeRenderPoses;
}

/**
 * Gets the array of Eye Render Textures which are the textures
 * that get directly rendered to each eye in the HMD.
 *
 * @author Katianie
 * @date 7/5/2016
 *
 * @return null if it fails, else the eye render textures.
 */
OculusTexture** VRLayer::GetEyeRenderTextures()
{
	return myEyeRenderTextures;
}

/**
 * Gets eye render viewports.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return null if it fails, else the eye render viewports.
 */
ovrRecti* VRLayer::GetEyeRenderViewports()
{
	return myEyeRenderViewports;
}

/**
 * Gets the HMD.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The HMD.
 */
ovrHmd VRLayer::GetHMD()
{
	return myHMD;
}

/**
 * Gets OVR layer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The OVR layer.
 */
ovrLayerEyeFov& VRLayer::GetOvrLayer()
{
	return myOvrLayer;
}