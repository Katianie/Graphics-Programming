/************************************************************************************
Filename		:   main.h
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
#include "../Common/Headers/BasicVR.h"
#include "../Common/Headers/VRLayer.h"
#include "../Common/Headers/Material.h"
#include "../Common/Headers/Model.h"
#include "../Common/Headers/Camera.h"
#include "../Common/Headers/Scene.h"      

class UsingBasicVR : public BasicVR
{
protected:
	int myNumVRLayers;

public:
	///Constructor
	UsingBasicVR(HINSTANCE hinst);

	///Destructor
	virtual ~UsingBasicVR();

	///Functions
	void MainLoop();
};

