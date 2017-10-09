/************************************************************************************
Filename		:   Texture.h
Content			:	Class responsible for managing and containing image data.
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

//DDS format related constants.
static const size_t   OVR_DDS_PF_FOURCC = 0x4;
static const uint32_t OVR_DXT1_MAGIC_NUMBER = 0x31545844; // "DXT1"
static const uint32_t OVR_DXT2_MAGIC_NUMBER = 0x32545844; // "DXT2"
static const uint32_t OVR_DXT3_MAGIC_NUMBER = 0x33545844; // "DXT3"
static const uint32_t OVR_DXT4_MAGIC_NUMBER = 0x34545844; // "DXT4"
static const uint32_t OVR_DXT5_MAGIC_NUMBER = 0x35545844; // "DXT5"

//Contains the pixel format of the DDS file.
struct OVR_DDS_PIXELFORMAT
{
	uint32_t Size;
	uint32_t Flags;
	uint32_t FourCC;
	uint32_t RGBBitCount;
	uint32_t RBitMask;
	uint32_t GBitMask;
	uint32_t BBitMask;
	uint32_t ABitMask;
};

//Contains the file format along with other 
//image data related to the DDS file.
struct OVR_DDS_HEADER
{
	uint32_t				Size;
	uint32_t				Flags;
	uint32_t				Height;
	uint32_t				Width;
	uint32_t				PitchOrLinearSize;
	uint32_t				Depth;
	uint32_t				MipMapCount;
	uint32_t				Reserved1[11];
	OVR_DDS_PIXELFORMAT     PixelFormat;
	uint32_t				Caps;
	uint32_t				Caps2;
	uint32_t				Caps3;
	uint32_t				Caps4;
	uint32_t				Reserved2;
};

//The format of the texture, various combinations 
//of the below constants could be specified.
enum TextureFormat
{
	Texture_RGBA = 0x100,
	Texture_R = 0x200,
	Texture_A = 0x400,
	Texture_BGRA = 0x800,
	Texture_DXT1 = 0x1100,
	Texture_DXT3 = 0x1200,
	Texture_DXT5 = 0x1300,

	Texture_Depth32f = 0x10000,   // aliased as default Texture_Depth
	Texture_Depth24Stencil8 = 0x20000,
	Texture_Depth32fStencil8 = 0x40000,
	Texture_Depth16 = 0x80000,

	Texture_DepthMask = 0xf0000,
	Texture_TypeMask = 0xfff00,
	Texture_Compressed = 0x1000,
	Texture_SamplesMask = 0x00ff,

	Texture_RenderTarget = 0x100000,
	Texture_SampleDepth = 0x200000,
	Texture_GenMipmaps = 0x400000,
	Texture_SRGB = 0x800000,
	Texture_Mirror = 0x1000000,
	Texture_SwapTextureSet = 0x2000000,
};

//The load specifications for the texture, 
//various combinations of the below constants 
//could be specified.
enum TextureLoadFlags
{
	TextureLoad_SrgbAware = 0x0001,
	TextureLoad_Anisotropic = 0x0002,
	TextureLoad_MakePremultAlpha = 0x0004,
	TextureLoad_SwapTextureSet = 0x0008,
};

class Texture : public Object
{
protected:
	OVR::SysFile* myTextureFile;
	ID3D11Texture2D* myDxTexture;
	ID3D11ShaderResourceView* myDxShaderResourceView;
	ID3D11RenderTargetView* myDxTextureRenderTargetView;
	unsigned char* myPixelBuffer;
	int	mySizeWidth;
	int mySizeHeight;
	int myMipmapLevels;

public:
	///Constructors
	Texture(char* textureFileName = "\0");
	void Init(char* textureFileName);

	///Destructor
	virtual ~Texture();

	///Functions
	void FillTexture(unsigned char* pix);
	void LoadTexture(char* textureFileName);
	unsigned char* LoadTextureDDS(OVR::File* file, int& buffSize);

	///Getters
	ID3D11ShaderResourceView*& GetDxShaderResourceView();
	ID3D11Texture2D* GetDxTexture();
	ID3D11RenderTargetView* GetDxTextureRenderTargetView();
	int GetMipmapLevels();
	int GetSizeHeight();
	int	GetSizeWidth();

};