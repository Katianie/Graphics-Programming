/************************************************************************************
Filename		:   DataBuffer.cpp
Content			:	Implementation of the DataBuffer class.
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
#include "../Headers/DataBuffer.h"

/**
 * Constructor, Creates a DirextX Buffer - used for
 * passing data to the shader among other things.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] dxDevice		The DX Device.
 * @param bindFlags		    The bind flags.
 * @param buffer		    The data for the buffer.
 * @param size			    The byte width.
 */
DataBuffer::DataBuffer(ID3D11Device* dxDevice, D3D11_BIND_FLAG bindFlags, const void* buffer, size_t size)
{
	mySize = size;

	memset(&myDxBufferDescription, 0, sizeof(myDxBufferDescription));
	myDxBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	myDxBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	myDxBufferDescription.BindFlags = bindFlags;
	myDxBufferDescription.ByteWidth = (unsigned)size;

	myDxSubResourceData.pSysMem = buffer;
	myDxSubResourceData.SysMemPitch = 0;
	myDxSubResourceData.SysMemSlicePitch = 0;

	if (buffer != NULL)
	{
		dxDevice->CreateBuffer(&myDxBufferDescription, &myDxSubResourceData, &myDxBuffer);
	}
	else
	{
		dxDevice->CreateBuffer(&myDxBufferDescription, NULL, &myDxBuffer);
	}
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
DataBuffer::~DataBuffer()
{
	if (myDxBuffer != NULL)
	{
		myDxBuffer->Release();
		myDxBuffer = NULL;
	}
}

/**
 * Gets the DX Buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Buffer.
 */
ID3D11Buffer*& DataBuffer::GetDxBuffer()
{
	return myDxBuffer;
}

/**
 * Gets the DX Buffer Description.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Buffer Description.
 */
D3D11_BUFFER_DESC DataBuffer::GetDxBufferDescription()
{
	return myDxBufferDescription;
}

/**
 * Gets the DX Sub Resource Data.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The DX Sub Resource Data.
 */
D3D11_SUBRESOURCE_DATA DataBuffer::GetDxSubResourceData()
{
	return myDxSubResourceData;
}

/**
 * Gets the size of the Buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The size of the Buffer.
 */
size_t DataBuffer::GetSize()
{
	return mySize;
}