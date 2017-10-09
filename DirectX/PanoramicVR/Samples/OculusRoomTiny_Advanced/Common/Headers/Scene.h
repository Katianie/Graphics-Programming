/************************************************************************************
Filename		:   Scene.h
Content			:	Contains the whole virtual world/scene.
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
#include "../Headers/Model.h"

static const short MAX_THREADS = 10;

class Scene : public Object
{
protected:
	Model* mySceneModel;
	vector<char*> myFileNames;
	vector<char*> myFolderNames;
	vector<char*> myImageNames;
	char* myConfigFilePath;
	char* myVertexShaderPath;
	char* myPixelShaderPath;
	size_t myCurrentImageIndex;

public:
	///Constructors
	Scene(char* imageFilePath, char* txtToolPath, char* outputPath, char* configFilePath, char* vertexShaderPath, char* pixelShaderPath);
	void Init(char* imageFilePath, char* txtToolPath, char* outputPath, char* configFilePath, char* vertexShaderPath, char* pixelShaderPath);

	///Destructor
	virtual ~Scene();

	///Functions
	float CalculateIdealPanoramicHeight(int panoramicType, float textureHeight);
	float CalculateIdealPanoramicRadius(int panoramicType, float textureWidth);
	float CalculateIdealPhotosphereRadius(float textureWidth, float textureHeight);
	static unsigned int __stdcall ExecuteCommand(void* command);
	void LoadNextImage();
	void LoadPreviousImage();
	void LoadScene(char* textureName);
	void Render(XMMATRIX* projView, float red, float green, float blue, float alpha);
	char* RetrieveConfigData(int panoramicType, char* tagName);

	///Getters
	int GetCurrentImageIndex();
	Model* GetSceneModel();
	
};
