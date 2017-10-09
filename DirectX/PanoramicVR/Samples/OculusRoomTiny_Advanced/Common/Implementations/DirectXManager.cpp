/************************************************************************************
Filename		:   DirectXManager.cpp
Content			:	Implementation of the DirectXManager class.
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
#include "../Headers/DirectXManager.h"

/**
 * Private Constructor. This is called only once and is used
 * by RetrieveDirectXManager().
 *
 * @author Katianie
 * @date 7/4/2016
 */
DirectXManager::DirectXManager()
{
	myWindow = NULL;
	myHInstance = NULL;
	myUniformDataBuffer = NULL;
	myMainDepthBuffer = NULL;
	myDxDevice = NULL;
	myDxDeviceContext = NULL;
	myDxSwapChain = NULL;
	myDxBackBufferTexture = NULL;
	myDxBackBufferRenderTarget = NULL;
	myIsRunning = false;
	myWindowSizeWidth = 0;
	myWindowSizeHeight = 0;

	//Clear uniform data.
	for (int i = 0; i < UNIFORM_DATA_SIZE; i++)
	{
		myUniformData[i] = 0;
	}
}

/**
 * Singleton Constructor/Getter. This class is a singleton;
 * in other words, this function will call the constructor only 
 * once and then return that reference after.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return A reference to a DirectXManager.
 */
DirectXManager& DirectXManager::RetrieveDirectXManager()
{
	//Gaunted to be destroyed on exit.
	static DirectXManager mySingletonRef;

	return mySingletonRef;
}

/**
 * Destructor. Calls ReleaseDevice() and CloseWindow().
 *
 * @author Katianie
 * @date 7/4/2016
 */
DirectXManager::~DirectXManager()
{
	DirectXManager::ReleaseDevice();
	DirectXManager::CloseWindow();
}

/**
 * Destroys the window and unregisters the class.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void DirectXManager::CloseWindow()
{
	if (myWindow != NULL)
	{
		DestroyWindow(myWindow);
		UnregisterClassW(L"PanoramicVR", myHInstance);
		myWindow = NULL;
	}
}

/**
 * Handles the messages/updates to the WIN32 window.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return true if it succeeds, false if it fails.
 */
bool DirectXManager::HandleMessages()
{
	MSG message;

	//Grab the message if there is one waiting.
	while (PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return myIsRunning;
}

/**
 * Initializes the everything related to DirectX; Depth buffers, 
 * data buffers, and the swap chain.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param viewportWidth  Width of the viewport.
 * @param viewportHeight Height of the viewport.
 * @param pLuid			 The LUID that uniquely identifies the Graphics Device.
 * @param isWindowed	 true if this object is windowed.
 *
 * @return true if it succeeds, false if it fails.
 */
bool DirectXManager::InitDevice(int viewportWidth, int viewportHeight, const LUID* pLuid, bool isWindowed)
{
	const UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW;
	RECT size = { 0, 0, viewportWidth, viewportHeight };
	HRESULT hResult = ERROR_SUCCESS;
	IDXGIFactory* dxGIFactory = NULL;
	IDXGIAdapter* dxAdapter = NULL;
	DXGI_ADAPTER_DESC dxAdapterDescription;
	IDXGIDevice1* dxGIDevice1 = NULL;
	D3D_DRIVER_TYPE dxDriverType;
	DXGI_SWAP_CHAIN_DESC dxSwapchainDescription;

	myWindowSizeWidth = viewportWidth;
	myWindowSizeHeight = viewportHeight;

	//Initialize the window to the size provided.
	AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW, false);

	//Set the position of the window along with the size and any options for initialization.
	if (SetWindowPos(myWindow, NULL, 0, 0, size.right - size.left, size.bottom - size.top, flags) == false)
	{
		return false;
	}

	hResult = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&dxGIFactory));
	AceDX::Validate((hResult == ERROR_SUCCESS), "CreateDXGIFactory1 failed");

	for (UINT adapterIndex = 0; dxGIFactory->EnumAdapters(adapterIndex, &dxAdapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		dxAdapter->GetDesc(&dxAdapterDescription);

		if ((pLuid == nullptr) || memcmp(&dxAdapterDescription.AdapterLuid, pLuid, sizeof(LUID)) == 0)
		{
			break;
		}

		delete dxAdapter;
		dxAdapter = NULL;
	}

	if (dxAdapter != NULL)
	{
		dxDriverType = D3D_DRIVER_TYPE_UNKNOWN;
	}
	else
	{
		dxDriverType = D3D_DRIVER_TYPE_HARDWARE;
	}

	hResult = D3D11CreateDevice(dxAdapter, dxDriverType, 0, 0, 0, 0, D3D11_SDK_VERSION, &myDxDevice, 0, &myDxDeviceContext);
	AceDX::Validate((hResult == ERROR_SUCCESS), "D3D11CreateDevice failed");

	//Create swap chain		
	memset(&dxSwapchainDescription, 0, sizeof(dxSwapchainDescription));
	dxSwapchainDescription.BufferCount = 2;
	dxSwapchainDescription.BufferDesc.Width = myWindowSizeWidth;
	dxSwapchainDescription.BufferDesc.Height = myWindowSizeHeight;
	dxSwapchainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxSwapchainDescription.BufferDesc.RefreshRate.Denominator = 1;
	dxSwapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxSwapchainDescription.OutputWindow = myWindow;
	dxSwapchainDescription.SampleDesc.Count = 1;
	dxSwapchainDescription.Windowed = isWindowed;
	dxSwapchainDescription.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	hResult = dxGIFactory->CreateSwapChain(myDxDevice, &dxSwapchainDescription, &myDxSwapChain);
	AceDX::Validate((hResult == ERROR_SUCCESS), "CreateSwapChain failed");

	//Create BackBufferTexture which is the modern "Frame Buffer".
	myDxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&myDxBackBufferTexture);
	hResult = myDxDevice->CreateRenderTargetView(myDxBackBufferTexture, NULL, &myDxBackBufferRenderTarget);
	AceDX::Validate((hResult == ERROR_SUCCESS), "CreateRenderTargetView failed");

	//Create the MainDepthBuffer which is the modern "Z-Buffer".
	myMainDepthBuffer = new DepthBuffer(myDxDevice, myWindowSizeWidth, myWindowSizeHeight);
	myDxDeviceContext->OMSetRenderTargets(1, &myDxBackBufferRenderTarget, myMainDepthBuffer->GetDxTextureDepthStencilView());

	//Buffer for shader constants
	myUniformDataBuffer = new DataBuffer(myDxDevice, D3D11_BIND_CONSTANT_BUFFER, NULL, UNIFORM_DATA_SIZE);
	myDxDeviceContext->VSSetConstantBuffers(0, 1, &myUniformDataBuffer->GetDxBuffer());

	//Set max frame latency to 1, Used for the modern "Double Buffering".
	hResult = myDxDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxGIDevice1);
	dxGIDevice1->SetMaximumFrameLatency(1);
	AceDX::Validate((hResult == ERROR_SUCCESS), "QueryInterface failed");

	dxAdapter->Release();
	dxGIFactory->Release();
	dxGIDevice1->Release();

	dxAdapter = NULL;
	dxGIFactory = NULL;
	dxGIDevice1 = NULL;

	return true;
}

/**
 * Initialize the DirectX viewport.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param viewportX		 The viewport x coordinate.
 * @param viewportY		 The viewport y coordinate.
 * @param viewportWidth  Width of the viewport.
 * @param viewportHeight Height of the viewport.
 */
void DirectXManager::InitViewport(float viewportX, float viewportY, float viewportWidth, float viewportHeight)
{
	D3D11_VIEWPORT dxViewport;

	dxViewport.Width = viewportWidth;
	dxViewport.Height = viewportHeight;
	dxViewport.MinDepth = 0;
	dxViewport.MaxDepth = 1;
	dxViewport.TopLeftX = viewportX;
	dxViewport.TopLeftY = viewportY;

	myDxDeviceContext->RSSetViewports(1, &dxViewport);
}

/**
 * Initialize the Win32 Window.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hInstance The handle to the current instance of this application.
 * @param title The title to use in the title bar of the window.
 *
 * @return true if it succeeds, false if it fails.
 */
bool DirectXManager::InitWindow(HINSTANCE hInstance, LPCWSTR title)
{
	WNDCLASSW windowDescription;

	myHInstance = hInstance;
	myIsRunning = true;

	memset(&windowDescription, 0, sizeof(windowDescription));
	windowDescription.lpszClassName = L"PanoramicVR";
	windowDescription.style = CS_OWNDC;
	windowDescription.lpfnWndProc = WindowProc;
	windowDescription.cbWndExtra = sizeof(this);
	RegisterClassW(&windowDescription);

	//Adjust the window size and show at InitDevice time.
	myWindow = CreateWindowW(windowDescription.lpszClassName, title, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, hInstance, 0);
	if (myWindow != NULL)
	{
		SetWindowLongPtr(myWindow, 0, LONG_PTR(this));
		return true;
	}

	return false;
}

/**
 * Releases the device. Delete and clean up anything related to DirectX
 * and close the window.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void DirectXManager::ReleaseDevice()
{
	if (myDxBackBufferTexture != NULL)
	{
		myDxBackBufferTexture->Release();
		myDxBackBufferTexture = NULL;
	}
	if (myDxBackBufferRenderTarget != NULL)
	{
		myDxBackBufferRenderTarget->Release();
		myDxBackBufferRenderTarget = NULL;
	}
	if (myDxDeviceContext != NULL)
	{
		myDxDeviceContext->Release();
		myDxDeviceContext = NULL;
	}
	if (myDxDevice != NULL)
	{
		myDxDevice->Release();
		myDxDevice = NULL;
	}
	if (myDxSwapChain)
	{
		myDxSwapChain->SetFullscreenState(FALSE, NULL);
		myDxSwapChain->Release();
		myDxSwapChain = NULL;
	}
	if (myMainDepthBuffer != NULL)
	{
		delete myMainDepthBuffer;
		myMainDepthBuffer = NULL;
	}
	if (myUniformDataBuffer != NULL)
	{
		delete myUniformDataBuffer;
		myUniformDataBuffer = NULL;
	}
	if (myWindow != NULL)
	{
		DirectXManager::CloseWindow();
	}
}

/**
 * Sets and clears render target, also sets the color to use
 * to paint the back buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] rendertarget If non-null, the rendertarget.
 * @param [in,out] depthbuffer  If non-null, the depthbuffer.
 * @param R					    The red float value to paint the back buffer.
 * @param G					    The green float value to paint the back buffer.
 * @param B					    The blue float value to paint the back buffer.
 * @param A					    The alpha float value to process.
 */
void DirectXManager::SetAndClearRenderTarget(ID3D11RenderTargetView* rendertarget, DepthBuffer* depthbuffer, float R, float G, float B, float A)
{
	//Important that alpha=0, if want pixels to be transparent, for manual layers
	float black[] = { R, G, B, A };

	if (depthbuffer != NULL)
	{
		myDxDeviceContext->OMSetRenderTargets(1, &rendertarget, depthbuffer->GetDxTextureDepthStencilView());
	}
	else
	{
		myDxDeviceContext->OMSetRenderTargets(1, &rendertarget, NULL);
	}

	myDxDeviceContext->ClearRenderTargetView(rendertarget, black);

	if (depthbuffer != NULL)
	{
		myDxDeviceContext->ClearDepthStencilView(depthbuffer->GetDxTextureDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
}

/**
 * Gets called whenever anything happens to the window; from clicking,
 * to key presses, or any message, its basically a handler function. 
 * See Win32 API to learn more about windows messages.
 * 
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx
 * 
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hWnd   A handle to the window.
 * @param Msg    The message.
 * @param wParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @param lParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter. 
 *
 * @return The return value is the result of the message processing and depends on the message sent.
 */
LRESULT CALLBACK DirectXManager::WindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	//Can't use "this" in a static function, here is a work-a-round.
	DirectXManager* currDxWindow = reinterpret_cast<DirectXManager*>(GetWindowLongPtr(hWnd, 0));

	//Handle when a key press/release is detected (not the key itself).
	if (Msg == WM_ACTIVATEAPP)
	{
		Keyboard::ProcessMessage(Msg, wParam, lParam);
	}
	if (Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN)
	{
		Keyboard::ProcessMessage(Msg, wParam, lParam);
	}
	if (Msg == WM_KEYUP || Msg == WM_SYSKEYUP)
	{
		Keyboard::ProcessMessage(Msg, wParam, lParam);
	}

	//Handle when user is closing the application.
	if (Msg == VK_ESCAPE || Msg == WM_QUIT || Msg == WM_CLOSE)
	{
		currDxWindow->myIsRunning = false;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/**
 * Gets the pixel buffer aka Back Buffer Render Target.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the pixel buffer aka Back Buffer Render Target.
 */
ID3D11RenderTargetView* DirectXManager::GetDxBackBufferRenderTarget()
{
	return myDxBackBufferRenderTarget;
}

/**
 * Gets the DX Back Buffer Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the DX Back Buffer Texture.
 */
ID3D11Texture2D* DirectXManager::GetDxBackBufferTexture()
{
	return myDxBackBufferTexture;
}

/**
 * Gets DX Device.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the DX Device.
 */
ID3D11Device* DirectXManager::GetDxDevice()
{
	return myDxDevice;
}

/**
 * Gets the DX Device Context.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the DX Device Context.
 */
ID3D11DeviceContext* DirectXManager::GetDxDeviceContext()
{
	return myDxDeviceContext;
}

/**
 * Gets the DX Swap Chain.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the DX Swap Chain.
 */
IDXGISwapChain* DirectXManager::GetDxSwapChain()
{
	return myDxSwapChain;
}

/**
 * Gets the hInstance (handle to the current application).
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The hInstance (handle to the current application).
 */
HINSTANCE DirectXManager::GetHInstance()
{
	return myHInstance;
}

/**
 * Gets IsRunning.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return true if it succeeds, false if it fails.
 */
bool DirectXManager::GetIsRunning()
{
	return myIsRunning;
}

/**
 * Gets the Depth Buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Depth Buffer.
 */
DepthBuffer* DirectXManager::GetMainDepthBuffer()
{
	return myMainDepthBuffer;
}

/**
 * Gets uniform data which is sent to the shader.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return uniform data which is sent to the shader.
 */
unsigned char* DirectXManager::GetUniformData()
{
	return myUniformData;
}

/**
 * Gets uniform data buffer which is sent to the shader.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the uniform data buffer which is sent to the shader.
 */
DataBuffer* DirectXManager::GetUniformDataBuffer()
{
	return myUniformDataBuffer;
}

/**
 * Gets the window.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The window.
 */
HWND DirectXManager::GetWindow()
{
	return myWindow;
}

/**
 * Gets window size height.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The window size height.
 */
int DirectXManager::GetWindowSizeHeight()
{
	return myWindowSizeHeight;
}

/**
 * Gets window size width.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The window size width.
 */
int DirectXManager::GetWindowSizeWidth()
{
	return myWindowSizeWidth;
}

/**
 * Set the data we want to send to the shader such as 
 * Model, View, Projection matrices, screen culling 
 * and pixel rejection.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param offset The offset index.
 * @param data   The data you want to send.
 * @param size   The size in bytes of the data.
 */
void DirectXManager::SetUniformData(int offset, const void* data, int size)
{
	if (offset >= 0 && offset < UNIFORM_DATA_SIZE && data != NULL && size > 0 && (size + offset) < UNIFORM_DATA_SIZE)
	{
		memcpy(myUniformData + offset, data, size);
	}
}


