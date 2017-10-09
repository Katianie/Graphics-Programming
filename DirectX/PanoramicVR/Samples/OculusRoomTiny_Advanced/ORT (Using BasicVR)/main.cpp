/************************************************************************************
Filename		:   main.cpp
Content			:	Main entry point for the application.
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
#include "../Common/Headers/stdafx.h"
#include "main.h"

/** Needs to be visible to WinMain(). */
UsingBasicVR* myApplication;

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hinst The hinst.
 */
UsingBasicVR::UsingBasicVR(HINSTANCE hinst) : BasicVR(hinst, L"PanoramicVR")
{
	//"We fear the future, lone for the past, and ignore the present."
	myNumVRLayers = 0;
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
UsingBasicVR::~UsingBasicVR()
{
	if (myApplication != NULL)
	{
		delete myApplication;
		myApplication = NULL;
	}

	myNumVRLayers = 0;
	BasicVR::~BasicVR();
}

/**
 * This is the main render loop for the application. 
 * It takes the rendered scene and renders it to
 * each eye in the HMD. Each eye has a VR Layer.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void UsingBasicVR::MainLoop()
{
	//there is only ever one vr layer...i think
	myVRLayers[myNumVRLayers++] = new VRLayer(myHMD);

	while (BasicVR::HandleMessages() == true)
	{
		for (int currVRLayer = 0; currVRLayer < myNumVRLayers; currVRLayer++)
		{
			BasicVR::ActionFromInput();
			myVRLayers[currVRLayer]->CalculateEyePoses();

			for (int eyeIndex = 0; eyeIndex < 2; eyeIndex++)
			{
				myVRLayers[currVRLayer]->RenderSceneToEyeBuffer(myMainCamera, myRoomScene, eyeIndex);
			}

			myVRLayers[currVRLayer]->InitializeLayerHeader();
			DistortAndPresent(1);
		}
	}
}

/**
 * ENTRY POINT FOR WINDOWS OS (See WinMain from Win32 API).
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * https://msdn.microsoft.com/library/windows/desktop/ms633559
 * 
 * @param hInstance		A handle to the current instance of the application.
 * @param hPrevInstance A handle to the previous instance of the application.
 * @param lpCmdLine		The command line for the application, excluding the program name.
 * @param nCmdShow		Constants to specify how the window is shown initially.
 *
 * @return 0 on success.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int retVal = 0;

	myApplication = new UsingBasicVR(hInstance);
	retVal = myApplication->Run();
	delete myApplication;

	return retVal;
}