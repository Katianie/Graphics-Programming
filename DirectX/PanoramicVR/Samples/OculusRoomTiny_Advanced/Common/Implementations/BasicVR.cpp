/************************************************************************************
Filename		:   BasicVR.cpp
Content			:	Implementation of the BasicVR class.
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
#include "../Headers/BasicVR.h"

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hInst		  The instance.
 * @param windowTitle The window title.
 */
BasicVR::BasicVR(HINSTANCE hInst, LPCWSTR windowTitle)
{
	myHInstance = hInst;
	myWindowTitle = windowTitle;
	myHMD = NULL;
	myCurrentResult = ovrSuccess;
	myMirrorTexture = NULL;
	myMainCamera = NULL;
	myRoomScene = NULL;
	myIsInitialized = false;
	myIsWindowed = true;
	myWindowWidthScale = 0.5f;
	myWindowHeightScale = 0.5f;
	myMirrorWidthScale = 1.0f;
	myMirrorHeightScale = 1.0f;
	myImageFilePath		= "..\\..\\..\\..\\..\\..\\Images\\";
	myTextureToolPath	= "..\\..\\..\\..\\..\\..\\Tools\\texconv";
	myTextureOutputPath	= "..\\..\\..\\..\\..\\..\\Textures";
	myConfigFilePath	= "..\\..\\..\\..\\..\\..\\Config.xml";
	myVertexShaderPath	= "..\\..\\..\\..\\..\\..\\SimpleVertexShader.txt";
	myPixelShaderPath	= "..\\..\\..\\..\\..\\..\\SimplePixelShader.txt";

	for (int i = 0; i < MAX_LAYERS; i++)
	{
		myVRLayers[i] = NULL;
	}

	//Initializes LibOVR, and the Rift
	myCurrentResult = ovr_Initialize(NULL);
	AceDX::Validate(OVR_SUCCESS(myCurrentResult), "Failed to initialize libOVR.");

	//Initializes the WinAPI window.
	AceDX::Validate(DirectXManager::RetrieveDirectXManager().InitWindow(myHInstance, myWindowTitle), "Failed to open window.");

	//Initialize the Oculus Rift and OVR.
	AceDX::Validate(BasicVR::Init(), "Oculus Rift not detected.");
}

/**
 * Initializes the Scene, Camera, Keyboard, and the Mirror window.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return true if initialization was successful, false if it fails.
 */
bool BasicVR::Init()
{
	ovrGraphicsLuid graphicsLUID;
	D3D11_TEXTURE2D_DESC mirrorTextureDescription = {};

	//Fill the HMD with initialization data.
	myCurrentResult = ovr_Create(&myHMD, &graphicsLUID);
	if (OVR_SUCCESS(myCurrentResult) == true)
	{
		//Setup Device and Graphics
		myHMDDescription = ovr_GetHmdDesc(myHMD);
		if (DirectXManager::RetrieveDirectXManager().InitDevice(int(myWindowWidthScale * myHMDDescription.Resolution.w), int(myWindowHeightScale * myHMDDescription.Resolution.h), reinterpret_cast<LUID*>(&graphicsLUID), myIsWindowed) == true)
		{
			//Create a mirror, to see Rift output on a monitor
			myMirrorTexture = NULL;
			mirrorTextureDescription.ArraySize = 1;
			mirrorTextureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			mirrorTextureDescription.Width = int(myMirrorWidthScale * DirectXManager::RetrieveDirectXManager().GetWindowSizeWidth());
			mirrorTextureDescription.Height = int(myMirrorHeightScale * DirectXManager::RetrieveDirectXManager().GetWindowSizeHeight());
			mirrorTextureDescription.Usage = D3D11_USAGE_DEFAULT;
			mirrorTextureDescription.SampleDesc.Count = 1;
			mirrorTextureDescription.MipLevels = 1;

			myCurrentResult = ovr_CreateMirrorTextureD3D11(myHMD, DirectXManager::RetrieveDirectXManager().GetDxDevice(), &mirrorTextureDescription, 0, &myMirrorTexture);
			if (OVR_SUCCESS(myCurrentResult) == true)
			{
				//Create the room scene.
				myRoomScene = new Scene(myImageFilePath, myTextureToolPath, myTextureOutputPath, myConfigFilePath, myVertexShaderPath, myPixelShaderPath);

				//Create the main camera.
				myMainCamera = new Camera(&XMVectorSet(1.0f, 1.6f, -2.0f, 1.0), &XMQuaternionIdentity());

				//Create the keyboard.
				myKeyboard = new Keyboard();
				myKeyboardStateTracker = new Keyboard::KeyboardStateTracker();

				//Successfully initialized.
				myIsInitialized = true;
			}
		}
	}

	return myIsInitialized;
}

/**
 * Destructor, Calls ReleaseDevice().
 *
 * @author Katianie
 * @date 7/4/2016
 */
BasicVR::~BasicVR()
{
	BasicVR::ReleaseDevice();
}

/**
 * Listens for keyboard input and when Right or Left is pressed,
 * the current scene is destroyed and then the next/previous scene
 * is loaded.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void BasicVR::ActionFromInput()
{
	myKeyboardStateTracker->Update(myKeyboard->GetState());

	if (myKeyboardStateTracker->pressed.Right == true)
	{
		myRoomScene->LoadNextImage();
	}
	else if (myKeyboardStateTracker->pressed.Left == true)
	{
		myRoomScene->LoadPreviousImage();
	}

	if (myKeyboardStateTracker->pressed.Escape)
	{
		this->ReleaseDevice();
	}
}

/**
 * Distort and present.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param numLayersToRender Number of layers to render.
 */
void BasicVR::DistortAndPresent(int numLayersToRender)
{
	//Collect the layer headers to send to the HMD.
	ovrLayerHeader* layerHeaders[MAX_LAYERS];
	for (int i = 0; i < MAX_LAYERS; i++)
	{
		if (myVRLayers[i] != NULL)
		{
			layerHeaders[i] = &myVRLayers[i]->GetOvrLayer().Header;
		}
	}

	//Submit the current texture and layer headers through 
	//the OVR pipeline to undergo distortion and timewarp.
	ovr_SubmitFrame(myHMD, 0, NULL, layerHeaders, numLayersToRender);

	//Take the current texture on the HMD and display it in the Mirror window, then swap buffers.
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->CopySubresourceRegion(DirectXManager::RetrieveDirectXManager().GetDxBackBufferTexture(), 0, 0, 0, 0, ((ovrD3D11Texture*)myMirrorTexture)->D3D11.pTexture, 0, NULL);
	DirectXManager::RetrieveDirectXManager().GetDxSwapChain()->Present(0, 0);
}

/**
 * Handles windows messages.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return true if it succeeds, false if it fails.
 */
bool BasicVR::HandleMessages()
{
	if (OVR_SUCCESS(myCurrentResult) == true)
	{
		if (DirectXManager::RetrieveDirectXManager().HandleMessages() == true)
		{
			return true;
		}
	}

	return false;
}

/**
 * Destroy and delete all objects and release the device.
 * Also shuts down OVR and exits the program with exit(0).
 *
 * @author Katianie
 * @date 7/4/2016
 */
void BasicVR::ReleaseDevice()
{
	myIsInitialized = false;

	if (myMirrorTexture != NULL)
	{
		ovr_DestroyMirrorTexture(myHMD, myMirrorTexture);
		myMirrorTexture = NULL;
	}

	if (myRoomScene != NULL)
	{
		delete myRoomScene;
		myRoomScene = NULL;
	}

	for (int i = 0; i < MAX_LAYERS; ++i)
	{
		if (myVRLayers[i] != NULL)
		{
			delete myVRLayers[i];
			myVRLayers[i] = NULL;
		}
	}

	if (myMainCamera != NULL)
	{
		delete myMainCamera;
		myMainCamera = NULL;
	}

	if (myKeyboard != NULL)
	{
		delete myKeyboard;
		myKeyboard = NULL;
	}

	if (myKeyboardStateTracker != NULL)
	{
		delete myKeyboardStateTracker;
		myKeyboardStateTracker = NULL;
	}

	DirectXManager::RetrieveDirectXManager().ReleaseDevice();

	if (myHMD != NULL)
	{
		ovr_Destroy(myHMD);
		myHMD = NULL;
	}

	ovr_Shutdown();
	exit(0);
}


/**
 * The main loop for the application. This is triggered in
 * the child classes (i.e. main.cpp).
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return 0 on success.
 */
int BasicVR::Run()
{
	while (DirectXManager::RetrieveDirectXManager().HandleMessages() == true)
	{
		if (myIsInitialized == true)
		{
			myCurrentResult = ovrSuccess;
			MainLoop();
			BasicVR::ReleaseDevice();
		}

		//True => retry on ovrError_DisplayLost
		if (BasicVR::Init() == false)
		{
			break;
		}
	}

	OutputDebugStringA("Run: Done\n");
	return 0;
}