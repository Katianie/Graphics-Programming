/************************************************************************************
Filename		:   DataBuffer.h
Content			:	Class responsible for containing raw data for the shader.
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

class DataBuffer : public Object
{
protected:
	D3D11_BUFFER_DESC myDxBufferDescription;
	D3D11_SUBRESOURCE_DATA myDxSubResourceData;
	ID3D11Buffer* myDxBuffer;
	size_t mySize;

public:
	///Constructor
	DataBuffer(ID3D11Device* dxDevice, D3D11_BIND_FLAG bindFlags, const void* buffer, size_t size);

	///Destructor
	virtual ~DataBuffer();

	///Getters
	ID3D11Buffer*& GetDxBuffer();
	D3D11_BUFFER_DESC GetDxBufferDescription();
	D3D11_SUBRESOURCE_DATA GetDxSubResourceData();
	size_t GetSize();
};