/************************************************************************************
Filename		:   Texture.cpp
Content			:	Implementation of the Texture class.
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
#include "../Headers/Texture.h"

/**
 * Constructor, calls LoadTexture().
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] textureFileName The file name of the texture to load.
 */
Texture::Texture(char* textureFileName)
{
	myTextureFile = NULL;
	myDxTexture = NULL;
	myDxShaderResourceView = NULL;
	myDxTextureRenderTargetView = NULL;
	myPixelBuffer = NULL;
	myMipmapLevels = 1;

	Texture::LoadTexture(textureFileName);
}

/**
 * Call Init() after we load the texture so we know the width, height, etc. See LoadTexture().
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] textureFileName If non-null, filename of the texture file.
 */
void Texture::Init(char* textureFileName)
{
	D3D11_TEXTURE2D_DESC textureDescription;

	textureDescription.Width = mySizeWidth;
	textureDescription.Height = mySizeHeight;
	textureDescription.MipLevels = myMipmapLevels;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateTexture2D(&textureDescription, NULL, &myDxTexture);
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateShaderResourceView(myDxTexture, NULL, &myDxShaderResourceView);
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Texture::~Texture()
{
	if (myTextureFile != NULL)
	{
		//We're using "Placement new" so we must call the destructor explicitly.
		myTextureFile->~SysFile();
		_aligned_free(myTextureFile);
		myTextureFile = NULL;
	}
	if (myDxTextureRenderTargetView != NULL)
	{
		myDxTextureRenderTargetView->Release();
		myDxTextureRenderTargetView = NULL;
	}
	if (myDxShaderResourceView != NULL)
	{
		myDxShaderResourceView->Release();
		myDxShaderResourceView = NULL;
	}
	if (myDxTexture != NULL)
	{
		myDxTexture->Release();
		myDxTexture = NULL;
	}
	if (myPixelBuffer != NULL)
	{
		_aligned_free(myPixelBuffer);
		myPixelBuffer = NULL;
	}
}

/**
 * Updates texture with the specified pixel buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] The pixel buffer.
 */
void Texture::FillTexture(unsigned char* pix)
{
	//FORMAT IS IN ABGR
	//Make local ones, because will be reducing them
	int sizeW = mySizeWidth;
	int sizeH = mySizeHeight;
	for (int level = 0; level < myMipmapLevels; level++)
	{
		DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->UpdateSubresource(myDxTexture, level, NULL, pix, sizeW * 4, sizeH * 4);

		for (int currColumn = 0; currColumn < (sizeH & ~1); currColumn += 2)
		{
			uint8_t* psrc = (uint8_t *)pix + (currColumn * sizeW * 4);
			uint8_t* pdest = (uint8_t *)pix + (currColumn * sizeW);
			for (int currRow = 0; currRow < sizeW >> 1; currRow++, psrc += 8, pdest += 4)
			{
				pdest[0] = (((int)psrc[0]) + psrc[4] + psrc[sizeW * 4 + 0] + psrc[sizeW * 4 + 4]) >> 2;
				pdest[1] = (((int)psrc[1]) + psrc[5] + psrc[sizeW * 4 + 1] + psrc[sizeW * 4 + 5]) >> 2;
				pdest[2] = (((int)psrc[2]) + psrc[6] + psrc[sizeW * 4 + 2] + psrc[sizeW * 4 + 6]) >> 2;
				pdest[3] = (((int)psrc[3]) + psrc[7] + psrc[sizeW * 4 + 3] + psrc[sizeW * 4 + 7]) >> 2;
			}
		}

		sizeW >>= 1;
		sizeH >>= 1;
	}
}

/**
 * Loads a texture from the specified file name.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] textureFileName The filename of the texture file.
 */
void Texture::LoadTexture(char* textureFileName)
{
	int buffSize = 0;

	if (textureFileName != NULL)
	{
		//I want to align the memory but also call the parameterized constructor 
		//to pass textureFileName. This technique is called "Placement new"
		myTextureFile = (OVR::SysFile*)_aligned_malloc(sizeof(OVR::SysFile), BYTE_ALIGNMENT);
		::new (myTextureFile) OVR::SysFile(textureFileName);

		if (myTextureFile->IsValid() == true)
		{
			//Ensure we deleted the previous pixel buffer before loading a new image.
			if (myPixelBuffer != NULL)
			{
				_aligned_free(myPixelBuffer);
				myPixelBuffer = NULL;
			}

			myPixelBuffer = Texture::LoadTextureDDS(myTextureFile, buffSize);
			Texture::Init(textureFileName);
			Texture::FillTexture((unsigned char*)myPixelBuffer);
		}
	}
}

/**
 * Loads the file and allocates the texture pixel buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] file	    The file to load.
 * @param [in] buffSize Size of the buffer.
 *
 * @return The texture pixel buffer.
 */
unsigned char* Texture::LoadTextureDDS(OVR::File* file, int& buffSize)
{
	OVR_DDS_HEADER header;
	int format;
	uint32_t mipCount;
	unsigned char filecode[4];
	unsigned char* bytes = NULL;

	file->Read(filecode, 4);
	if (Ace::IndexOf((char*)filecode, "DDS") >= 0)
	{
		file->Read((unsigned char*)(&header), sizeof(header));

		mySizeWidth = header.Width;
		mySizeHeight = header.Height;
		format = Texture_SRGB;
		mipCount = header.MipMapCount;

		buffSize = file->BytesAvailable();
		bytes = (unsigned char*)_aligned_malloc(buffSize * sizeof(unsigned char), BYTE_ALIGNMENT);
		file->Read(bytes, buffSize);
		file->Close();
	}

	return bytes;
}

/**
 * Gets the DX Shader Resource View.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Shader Resource View.
 */
ID3D11ShaderResourceView*& Texture::GetDxShaderResourceView()
{
	return myDxShaderResourceView;
}

/**
 * Gets the DX Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Texture.
 */
ID3D11Texture2D* Texture::GetDxTexture()
{
	return myDxTexture;
}

/**
 * Gets the DX Texture Render Target View.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Texture Render Target View.
 */
ID3D11RenderTargetView* Texture::GetDxTextureRenderTargetView()
{
	return myDxTextureRenderTargetView;
}

/**
 * Gets the Mipmap levels of the texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Mipmap levels of the texture.
 */
int Texture::GetMipmapLevels()
{
	return myMipmapLevels;
}

/**
 * Gets the height of the Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The size height.
 */
int Texture::GetSizeHeight()
{
	return mySizeHeight;
}

/**
 * Gets the width of the Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The width of the Texture.
 */
int	Texture::GetSizeWidth()
{
	return mySizeWidth;
}