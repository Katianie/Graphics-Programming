/************************************************************************************
Filename		:   Model.cpp
Content			:	Implementation of the Model class.
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
#include "../Headers/Model.h"

/**
 * Default Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Model::Model()
{
	myFirstPosition = XMFLOAT4();
	myPosition = XMFLOAT3();
	myRotation = XMFLOAT4();
	myMaterial = NULL;
	myVertexBuffer = NULL;
	myIndexBuffer = NULL;
	myNumIndices = 0;
}

/**
 * Parameterized Constructor, also calls Init().
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param panoramicType			 Type of the panoramic (180, 270, 360, or 999 for photosphere).
 * @param [in,out] triangleSet   If non-null, set the triangle belongs to.
 * @param modelPosition			 The model position.
 * @param modelRotation			 The model rotation.
 * @param [in,out] modelMaterial If non-null, the model material.
 */
Model::Model(int panoramicType, TriangleSet* triangleSet, XMFLOAT3 modelPosition, XMFLOAT4 modelRotation, Material* modelMaterial)
{
	myPosition = modelPosition;
	myRotation = modelRotation;
	myMaterial = modelMaterial;

	Init(panoramicType, triangleSet);
}

/**
 * Initializes the model by taking in the geometry and the type
 * of panoramic (180, 270, 360, and 999 for photosphere).
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param panoramicType		   Type of the panoramic.
 * @param [in,out] triangleSet If non-null, set the triangle belongs to.
 */
void Model::Init(int panoramicType, TriangleSet* triangleSet)
{
	//Determine whether to cut off the repeating backing for 180 or 270 panoramic.
	if (panoramicType == 180 || panoramicType == 270)
	{
		if (triangleSet->GetVertices() != NULL)
		{
			Vertex firstPosition = triangleSet->GetVertices()[0];
			myFirstPosition = XMFLOAT4(firstPosition.GetX(), firstPosition.GetY(), firstPosition.GetZ(), 1.0);
		}
	}
	else
	{
		//Flag this model for no back clipping (360 and Photosphere).
		myFirstPosition.x = 999.9f;
		myFirstPosition.y = 999.9f;
		myFirstPosition.z = 999.9f;
	}

	myNumIndices = triangleSet->GetNumIndices();
	myVertexBuffer = new DataBuffer(DirectXManager::RetrieveDirectXManager().GetDxDevice(), D3D11_BIND_VERTEX_BUFFER, &triangleSet->GetVertices()[0], triangleSet->GetNumVertices() * sizeof(Vertex));
	myIndexBuffer = new DataBuffer(DirectXManager::RetrieveDirectXManager().GetDxDevice(), D3D11_BIND_INDEX_BUFFER, &triangleSet->GetIndices()[0], triangleSet->GetNumIndices() * sizeof(short));
}

/**
 * Destructor, Deletes the model and cleans up.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Model::~Model()
{
	if (myMaterial != NULL)
	{
		delete myMaterial;
		myMaterial = NULL;
	}
	
	if (myVertexBuffer != NULL)
	{
		delete myVertexBuffer;
		myVertexBuffer = NULL;
	}
	
	if (myIndexBuffer != NULL)
	{
		delete myIndexBuffer;
		myIndexBuffer = NULL;
	}
}

/**
 * Primary render function, renders the model (geometry, material, and shader data).
 * 
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] projView		The Model View Projection matrix.
 * @param red			    The red color value for the current pixel.
 * @param green			    The green color value for the current pixel.
 * @param blue			    The blue color value for the current pixel.
 * @param alpha			    The alpha color value for the current pixel.
 */
void Model::Render(XMMATRIX* projView, float red, float green, float blue, float alpha)
{
	UINT offset = 0;
	float angleOfRotation = myRotation.w;
	D3D11_MAPPED_SUBRESOURCE dxMappedResource;
	XMFLOAT4 masterColor = XMFLOAT4(red, green, blue, alpha);
	XMFLOAT3 rotationAxis = XMFLOAT3(myRotation.x, myRotation.y, myRotation.z);
	XMMATRIX modelMat = XMMatrixMultiply(DirectX::XMMatrixRotationAxis(XMLoadFloat3(&rotationAxis), angleOfRotation), XMMatrixTranslationFromVector(XMLoadFloat3(&myPosition)));
	XMMATRIX MVP = XMMatrixMultiply(modelMat, *projView);

	//Initialize shader data.
 	DirectXManager::RetrieveDirectXManager().SetUniformData(0, &MVP, sizeof(MVP));
	DirectXManager::RetrieveDirectXManager().SetUniformData(64, &masterColor, sizeof(masterColor));
	DirectXManager::RetrieveDirectXManager().SetUniformData(80, &modelMat, sizeof(modelMat));
	DirectXManager::RetrieveDirectXManager().SetUniformData(144, &myFirstPosition, sizeof(myFirstPosition));

	//Pass shader data to dxMappedResource.
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->Map(DirectXManager::RetrieveDirectXManager().GetUniformDataBuffer()->GetDxBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &dxMappedResource);
	memcpy(dxMappedResource.pData, DirectXManager::RetrieveDirectXManager().GetUniformData(), UNIFORM_DATA_SIZE);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->Unmap(DirectXManager::RetrieveDirectXManager().GetUniformDataBuffer()->GetDxBuffer(), 0);

	//Pass the vertex data to the Vertex Buffer Object.
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->IASetInputLayout(myMaterial->GetDxInputLayout());
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->IASetIndexBuffer(myIndexBuffer->GetDxBuffer(), DXGI_FORMAT_R16_UINT, 0);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->IASetVertexBuffers(0, 1, &myVertexBuffer->GetDxBuffer(), &myMaterial->GetVertexBufferSize(), &offset);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Pass material data to shader.
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->VSSetShader(myMaterial->GetDxVertexShader(), NULL, 0);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->PSSetShader(myMaterial->GetDxPixelShader(), NULL, 0);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->PSSetSamplers(0, 1, &myMaterial->GetDxSamplerState());
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->RSSetState(myMaterial->GetDxRasterizerState());
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->OMSetDepthStencilState(myMaterial->GetDxDepthState(), 0);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->OMSetBlendState(myMaterial->GetDxBlendState(), NULL, 0xffffffff);
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->PSSetShaderResources(0, 1, &myMaterial->GetTexture()->GetDxShaderResourceView());
	DirectXManager::RetrieveDirectXManager().GetDxDeviceContext()->DrawIndexed((UINT)myNumIndices, 0, 0);
}

/**
 * Gets the index buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The index buffer.
 */
DataBuffer* Model::GetIndexBuffer()
{
	return myIndexBuffer;
}

/**
 * Gets the material.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The material.
 */
Material* Model::GetMaterial()
{
	return myMaterial;
}

/**
 * Gets the number indices.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The number indices.
 */
int Model::GetNumIndices()
{
	return myNumIndices;
}

/**
 * Gets the position of the Model.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The position.
 */
XMFLOAT3 Model::GetPosition()
{
	return myPosition;
}

/**
 * Gets the rotation vector of the model.
 * The x,y,z components are the axis and the w
 * component is the angle of rotation in degrees.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The rotation vector and angle.
 */
XMFLOAT4 Model::GetRotation()
{
	return myRotation;
}

/**
 * Gets the vertex buffer.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The vertex buffer.
 */
DataBuffer* Model::GetVertexBuffer()
{
	return myVertexBuffer;
}

/**
 * Sets the position.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param pos The new position.
 */
void Model::SetPosition(XMFLOAT3 pos)
{
	myPosition = pos;
}