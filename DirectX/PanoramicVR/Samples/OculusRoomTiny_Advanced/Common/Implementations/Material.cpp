/************************************************************************************
Filename		:   Material.cpp
Content			:	Implementation of the Material class.
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
#include "../Headers/Material.h"

/**
 * Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] texture		    If non-null, the texture.
 * @param [in] vertexShaderPath If non-null, full pathname of the vertex shader file.
 * @param [in] pixelShaderPath  If non-null, full pathname of the pixel shader file.
 */
Material::Material(Texture* texture, char* vertexShaderPath, char* pixelShaderPath)
{
	char* vertexShaderSrc = NULL;
	char* pixelShaderSrc = NULL;
	ID3DBlob* blobData;
	D3D11_SAMPLER_DESC samplerState;
	D3D11_RASTERIZER_DESC rasterizerState;
	D3D11_DEPTH_STENCIL_DESC depthStencilState;
	D3D11_BLEND_DESC blendState;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color",    0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	myDxVertexShader = NULL;
	myDxPixelShader = NULL;
	myDxInputLayout = NULL;
	myDxSamplerState = NULL;
	myDxRasterizerState = NULL;
	myDxDepthState = NULL;
	myDxBlendState = NULL;
	myTexture = texture;
	myVertexBufferSize = 24;
	memset(&samplerState, 0, sizeof(samplerState));
	memset(&rasterizerState, 0, sizeof(rasterizerState));
	memset(&depthStencilState, 0, sizeof(depthStencilState));
	memset(&blendState, 0, sizeof(blendState));

	vertexShaderSrc = Ace::RetrieveFileData(vertexShaderPath, false);
	Ace::ConvertStringToOneLineInPlace(vertexShaderSrc);

	pixelShaderSrc = Ace::RetrieveFileData(pixelShaderPath, false);
	Ace::ConvertStringToOneLineInPlace(pixelShaderSrc);

	//Create vertex shader
	D3DCompile(vertexShaderSrc, strlen(vertexShaderSrc), 0, 0, 0, "main", "vs_4_0", 0, 0, &blobData, 0);
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &myDxVertexShader);

	//Create input layout
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateInputLayout(vertexDesc, 3, blobData->GetBufferPointer(), blobData->GetBufferSize(), &myDxInputLayout);
	blobData->Release();

	//Create pixel shader
	D3DCompile(pixelShaderSrc, strlen(pixelShaderSrc), 0, 0, 0, "main", "ps_4_0", 0, 0, &blobData, 0);
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &myDxPixelShader);
	blobData->Release();

	//We are done with shader source code now that they have been compiled.
	_aligned_free(vertexShaderSrc);
	_aligned_free(pixelShaderSrc);

	samplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerState.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerState.MaxAnisotropy = 8;
	samplerState.MaxLOD = 15;
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateSamplerState(&samplerState, &myDxSamplerState);

	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.DepthClipEnable = false;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.AntialiasedLineEnable = rasterizerState.DepthClipEnable;
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateRasterizerState(&rasterizerState, &myDxRasterizerState);

	depthStencilState.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilState.DepthEnable = true;
	depthStencilState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateDepthStencilState(&depthStencilState, &myDxDepthState);

	blendState.RenderTarget[0].BlendEnable = true;
	blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DirectXManager::RetrieveDirectXManager().GetDxDevice()->CreateBlendState(&blendState, &myDxBlendState);
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Material::~Material()
{
	if (myDxVertexShader != NULL)
	{
		myDxVertexShader->Release();
		myDxVertexShader = NULL;
	}
	if (myDxInputLayout != NULL)
	{
		myDxInputLayout->Release();
		myDxInputLayout = NULL;
	}
	if (myDxSamplerState != NULL)
	{
		myDxSamplerState->Release();
		myDxSamplerState = NULL;
	}
	if (myDxRasterizerState != NULL)
	{
		myDxRasterizerState->Release();
		myDxRasterizerState = NULL;
	}
	if (myDxDepthState != NULL)
	{
		myDxDepthState->Release();
		myDxDepthState = NULL;
	}
	if (myDxBlendState != NULL)
	{
		myDxBlendState->Release();
		myDxBlendState = NULL;
	}
	if (myTexture != NULL)
	{
		delete myTexture;
		myTexture = NULL;
	}
}

/**
 * Gets the DX Blend State.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Blend State.
 */
ID3D11BlendState* Material::GetDxBlendState()
{
	return myDxBlendState;
}

/**
 * Gets the DX Depth State.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Depth State.
 */
ID3D11DepthStencilState* Material::GetDxDepthState()
{
	return myDxDepthState;
}

/**
 * Gets the DX Input Layout.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Input Layout.
 */
ID3D11InputLayout* Material::GetDxInputLayout()
{
	return myDxInputLayout;
}

/**
 * Gets DX Pixel Shader.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Pixel Shader.
 */
ID3D11PixelShader* Material::GetDxPixelShader()
{
	return myDxPixelShader;
}

/**
 * Gets the DX Rasterizer State.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Rasterizer State.
 */
ID3D11RasterizerState* Material::GetDxRasterizerState()
{
	return myDxRasterizerState;
}

/**
 * Gets the DX Sampler State.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Sampler State.
 */
ID3D11SamplerState*& Material::GetDxSamplerState()
{
	return myDxSamplerState;
}

/**
 * Gets the DX Vertex Shader.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Vertex Shader.
 */
ID3D11VertexShader* Material::GetDxVertexShader()
{
	return myDxVertexShader;
}

/**
 * Gets the Texture.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Texture.
 */
Texture* Material::GetTexture()
{
	return myTexture;
}

/**
 * Gets the Vertex Buffer size.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The Vertex Buffer size.
 */
UINT& Material::GetVertexBufferSize()
{
	return myVertexBufferSize;
}



