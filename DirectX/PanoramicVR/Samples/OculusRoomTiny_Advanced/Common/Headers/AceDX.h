/************************************************************************************
Filename		:   AceDX.h
Content			:	Common utility functions related to OVR and DirectX.
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
#include "../Headers/Texture.h"

class AceDX
{
public:
	///Functions
	static XMMATRIX ConvertOVRMatrixToXMMatrix(ovrMatrix4f mat);
	static XMFLOAT4 ConvertOVRQuaternionToXMFloat4(ovrQuatf quat);
	static XMVECTOR ConvertOVRQuaternionToXMVector(ovrQuatf quat);
	static XMFLOAT3 ConvertOVRVector3ToXMFloat3(ovrVector3f vect);
	static XMVECTOR ConvertOVRVector3ToXMVector(ovrVector3f vect);
	static XMFLOAT3 ConvertStringToXMFloat3(char* str);
	static XMFLOAT4 ConvertStringToXMFloat4(char* str);
	static void Validate(bool expression, char* msgIfFail);
};