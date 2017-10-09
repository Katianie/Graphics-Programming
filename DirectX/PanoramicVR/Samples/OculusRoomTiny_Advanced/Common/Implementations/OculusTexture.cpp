/************************************************************************************
Filename		:   OculusTexture.cpp
Content			:	Implementation of the OculusTexture class.
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
#include "../Headers/OculusTexture.h"

/**
 * Default Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
OculusTexture::OculusTexture()
{
	myHMD = NULL;
	mySwapTextureSet = NULL;
	mySizeWidth = 0;
	mySizeHeight = 0;

	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		myRenderTargets[i] = NULL;
	}
}

/**
 * Initializes the OVR Texture and anything related to OVR
 * such as mySwapTextureSet.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param hmd		 The HMD.
 * @param sizeWidth  Width of the Texture.
 * @param sizeHeight Height of the Texture.
 *
 * @return true if it succeeds, false if it fails.
 */
bool OculusTexture::Init(ovrHmd hmd, int sizeWidth, int sizeHeight)
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDescription = {};
	D3D11_TEXTURE2D_DESC textureDescription;
	ovrResult result;
	ovrD3D11Texture* currTexture;

	myHMD = hmd;
	mySizeWidth = sizeWidth;
	mySizeHeight = sizeHeight;

	textureDescription.Width = mySizeWidth;
	textureDescription.Height = mySizeHeight;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDescription.SampleDesc.Count = 1;   // No multi-sampling allowed
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	result = ovr_CreateSwapTextureSetD3D11(hmd, DirectXManager::RetrieveDirectXManager().GetDxDevice(), &textureDescription, ovrSwapTextureSetD3D11_Typeless, &mySwapTextureSet);
	if (OVR_SUCCESS(result))
	{
		AceDX::Validate(mySwapTextureSet->TextureCount == TEXTURE_COUNT, "TextureCount mismatch.");

		for (int i = 0; i < TEXTURE_COUNT; i++)
		{
			currTexture = (ovrD3D11Texture*)&mySwapTextureSet->Textures[i];

			renderTargetDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			renderTargetDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateRenderTargetView(currTexture->D3D11.pTexture, &renderTargetDescription, &myRenderTargets[i]);
		}

		return true;
	}

	return false;
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
OculusTexture::~OculusTexture()
{
	for (int i = 0; i < TEXTURE_COUNT; ++i)
	{
		myRenderTargets[i]->Release();
		myRenderTargets[i] = NULL;
	}

	if (mySwapTextureSet != NULL)
	{
		ovr_DestroySwapTextureSet(myHMD, mySwapTextureSet);
		mySwapTextureSet = NULL;
	}
}

/**
 * Increment the index of the current swap texture.
 *
 * @author Katianie
 * @date 7/4/2016
 */
void OculusTexture::Increment()
{
	mySwapTextureSet->CurrentIndex = (mySwapTextureSet->CurrentIndex + 1) % mySwapTextureSet->TextureCount;
}

/**
 * Gets the HMD
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The HMD.
 */
ovrHmd OculusTexture::GetHMD()
{
	return myHMD;
}

/**
 * Gets the specified Render Target.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param index Zero-based index of the render target.
 *
 * @return The Render Target.
 */
ID3D11RenderTargetView* OculusTexture::GetRenderTarget(int index)
{
	if (index >= 0 && index < TEXTURE_COUNT)
	{
		return myRenderTargets[index];
	}

	return NULL;
}

/**
 * Gets the Texture height.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The size height.
 */
int OculusTexture::GetSizeHeight()
{
	return mySizeHeight;
}

/**
 * Gets the texture width.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Texture width.
 */
int OculusTexture::GetSizeWidth()
{
	return mySizeWidth;
}

/**
 * Gets the Swap Texture Set.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Swap Texture Set.
 */
ovrSwapTextureSet* OculusTexture::GetSwapTextureSet()
{
	return mySwapTextureSet;
}