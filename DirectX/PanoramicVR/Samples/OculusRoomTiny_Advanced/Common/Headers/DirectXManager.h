/************************************************************************************
Filename		:   DirectXManager.h
Content			:	Class responsible for handling all DirectX related functionality.
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
#include "../Headers/AceDX.h"
#include "../Headers/DataBuffer.h"
#include "../Headers/DepthBuffer.h"

//Fixed size buffer for shader constants, before copied into buffer
static const int UNIFORM_DATA_SIZE = 2000;

class DirectXManager : public Object
{	
protected:
	unsigned char myUniformData[UNIFORM_DATA_SIZE];
	HWND myWindow;
	HINSTANCE myHInstance;
	DataBuffer* myUniformDataBuffer;
	DepthBuffer* myMainDepthBuffer;
	ID3D11Device* myDxDevice;
	ID3D11DeviceContext* myDxDeviceContext;
	IDXGISwapChain* myDxSwapChain;
	ID3D11Texture2D* myDxBackBufferTexture;
	ID3D11RenderTargetView* myDxBackBufferRenderTarget;
	bool myIsRunning;
	int myWindowSizeWidth;
	int myWindowSizeHeight;

	///Private Constructor.
	DirectXManager();
	
public:
	///Singleton Constructor/Getter.
	static DirectXManager& RetrieveDirectXManager();

	///Destructor
	virtual ~DirectXManager();

	///Functions
	void CloseWindow();
	bool HandleMessages();
	bool InitDevice(int viewportWidth, int viewportHeight, const LUID* pLuid, bool isWindowed = true);
	void InitViewport(float viewportX, float viewportY, float viewportWidth, float viewportHeight);
	bool InitWindow(HINSTANCE hInstance, LPCWSTR title);
	void ReleaseDevice();
	void SetAndClearRenderTarget(ID3D11RenderTargetView* rendertarget, DepthBuffer* depthbuffer, float R = 0.0f, float G = 0.0f, float B = 0.0f, float A = 0.0f);
	static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	///Getters
	ID3D11RenderTargetView* GetDxBackBufferRenderTarget();
	ID3D11Texture2D* GetDxBackBufferTexture();
	ID3D11Device* GetDxDevice();
	ID3D11DeviceContext* GetDxDeviceContext();
	IDXGISwapChain* GetDxSwapChain();
	HINSTANCE GetHInstance();
	bool GetIsRunning();
	DepthBuffer* GetMainDepthBuffer();
	unsigned char* GetUniformData();
	DataBuffer* GetUniformDataBuffer();
	HWND GetWindow();
	int GetWindowSizeHeight();
	int GetWindowSizeWidth();

	///Setters
	void SetUniformData(int offset, const void* data, int size);
};
