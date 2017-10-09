/************************************************************************************
Filename		:   DepthBuffer.cpp
Content			:	Implementation of the DepthBuffer class.
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
#include "../Headers/DepthBuffer.h"

/**
 * Constructor, initializes buffer to store Z order data aka Z-Buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] dxDevice The DX Device.
 * @param windowWidth	    The width of the window.
 * @param windowHeight	    The height of the window.
 * @param sampleCount	    Number of samples.
 */
DepthBuffer::DepthBuffer(ID3D11Device* dxDevice, int windowWidth, int windowHeight, int sampleCount)
{
	myDxTextureDescription.Width = windowWidth;
	myDxTextureDescription.Height = windowHeight;
	myDxTextureDescription.MipLevels = 1;
	myDxTextureDescription.ArraySize = 1;
	myDxTextureDescription.Format = myDxFormat;
	myDxTextureDescription.SampleDesc.Count = sampleCount;
	myDxTextureDescription.SampleDesc.Quality = 0;
	myDxTextureDescription.Usage = D3D11_USAGE_DEFAULT;
	myDxTextureDescription.CPUAccessFlags = 0;
	myDxTextureDescription.MiscFlags = 0;
	myDxTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (dxDevice != nullptr)
	{
		dxDevice->CreateTexture2D(&myDxTextureDescription, NULL, &myDxTexture);
		dxDevice->CreateDepthStencilView(myDxTexture, NULL, &myDxTextureDepthStencilView);
	}
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
DepthBuffer::~DepthBuffer()
{
	if (myDxTextureDepthStencilView != NULL)
	{
		myDxTextureDepthStencilView->Release();
		myDxTextureDepthStencilView = NULL;
	}
}

/**
 * Gets the DX Format.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Format.
 */
DXGI_FORMAT DepthBuffer::GetDxFormat()
{
	return myDxFormat;
}

/**
 * Gets the DX Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the DX Texture.
 */
ID3D11Texture2D* DepthBuffer::GetDxTexture()
{
	return myDxTexture;
}

/**
 * Gets the DX Texture Depth Stencil View.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Texture Depth Stencil View.
 */
ID3D11DepthStencilView* DepthBuffer::GetDxTextureDepthStencilView()
{
	return myDxTextureDepthStencilView;
}

/**
 * Gets the DX Texture Description.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Texture Description.
 */
D3D11_TEXTURE2D_DESC DepthBuffer::GetDxTextureDescription()
{
	return myDxTextureDescription;
}