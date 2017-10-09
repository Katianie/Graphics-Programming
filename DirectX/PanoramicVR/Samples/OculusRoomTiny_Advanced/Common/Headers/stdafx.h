/************************************************************************************
Filename		:   stdafx.h
Content			:	Common headers for all classes.
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
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/stat.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <string>
#include <algorithm>
#include <future>
#include <chrono>
#include <cctype>
#include <cstdint>
#include <vector>
#include <new>
#include "d3dcompiler.h"
#include "d3d11.h"
#include "Ace.h"

#include "DirectXMath.h"
using namespace DirectX;

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

///DXTK (modern D3DX)
//#include "CommonStates.h"
#include "DDSTextureLoader.h"
//#include "DirectXHelpers.h"
//#include "Effects.h"
//#include "GamePad.h"
#include "GeometricPrimitive.h"
//#include "GraphicsMemory.h"
#include "Keyboard.h"
//#include "Model.h"
//#include "Mouse.h"
//#include "PrimitiveBatch.h"
//#include "ScreenGrab.h"
//#include "SimpleMath.h"
//#include "SpriteBatch.h"
//#include "SpriteFont.h"
#include "VertexTypes.h"
//#include "WICTextureLoader.h"

///OculusVR SDK
#include "Extras/OVR_Math.h"
#include "Kernel/OVR_Std.h"
#include "Kernel/OVR_Array.h"
#include "Kernel/OVR_RefCount.h"
#include "Kernel/OVR_String.h"
#include "Kernel/OVR_File.h"
#include "Kernel/OVR_SysFile.h"
#include "Kernel/OVR_Color.h"
#include "OVR_CAPI_D3D.h"  

using namespace std;

///Local Includes
#include "Object.h"
#include "AceDX.h"
#include "Vertex.h"
#include "DirectXManager.h"