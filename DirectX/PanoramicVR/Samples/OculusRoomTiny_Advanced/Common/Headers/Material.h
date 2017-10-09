/************************************************************************************
Filename		:   Material.h
Content			:	Class responsible for tyeing together the Texture and Shader Data.
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

class Material : public Object
{
protected:
	ID3D11VertexShader* myDxVertexShader;
	ID3D11PixelShader* myDxPixelShader;
	ID3D11InputLayout* myDxInputLayout;
	ID3D11SamplerState* myDxSamplerState;
	ID3D11RasterizerState* myDxRasterizerState;
	ID3D11DepthStencilState* myDxDepthState;
	ID3D11BlendState* myDxBlendState;
	Texture* myTexture;
	UINT myVertexBufferSize;

public:
	enum { MAT_WRAP = 1, MAT_WIRE = 2, MAT_ZALWAYS = 4, MAT_NOCULL = 8, MAT_TRANS = 16 };

	///Constructor
	Material(Texture* texture, char* vertexShaderPath = NULL, char* pixelShaderPath = NULL);
	
	///Destructor
	~Material();

	///Getters
	ID3D11VertexShader* GetDxVertexShader();
	ID3D11PixelShader* GetDxPixelShader();
	ID3D11InputLayout* GetDxInputLayout();
	ID3D11SamplerState*& GetDxSamplerState();
	ID3D11RasterizerState* GetDxRasterizerState();
	ID3D11DepthStencilState* GetDxDepthState();
	ID3D11BlendState* GetDxBlendState();
	Texture* GetTexture();
	UINT& GetVertexBufferSize();
};