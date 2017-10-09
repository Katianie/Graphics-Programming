/************************************************************************************
Filename		:   DepthBuffer.h
Content			:	Class responsible for containing data related to the Z-Buffer.
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

class DepthBuffer : public Object
{
protected:
	ID3D11DepthStencilView* myDxTextureDepthStencilView;
	ID3D11Texture2D* myDxTexture;
	DXGI_FORMAT myDxFormat = DXGI_FORMAT_D32_FLOAT;
	D3D11_TEXTURE2D_DESC myDxTextureDescription;

public:
	///Constructor
	DepthBuffer(ID3D11Device* dxDevice, int windowWidth, int windowHeight, int sampleCount = 1);

	///Destructor
	virtual ~DepthBuffer();

	///Getters
	DXGI_FORMAT GetDxFormat();
	ID3D11Texture2D* GetDxTexture();
	ID3D11DepthStencilView* GetDxTextureDepthStencilView();
	D3D11_TEXTURE2D_DESC GetDxTextureDescription();

};
