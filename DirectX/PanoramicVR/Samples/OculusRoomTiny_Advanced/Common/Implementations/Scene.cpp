/************************************************************************************
Filename		:   Scene.cpp
Content			:	Implementation of the Scene class.
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
#include "../Headers/Scene.h"

/**
 * Constructor, takes in all the file paths and passes them to Init().
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] imageFilePath    If non-null, full pathname of the image file.
 * @param [in,out] txtToolPath	    If non-null, full pathname of the text tool file.
 * @param [in,out] outputPath	    If non-null, full pathname of the output file.
 * @param [in,out] configFilePath   If non-null, full pathname of the configuration file.
 * @param [in,out] vertexShaderPath If non-null, full pathname of the vertex shader file.
 * @param [in,out] pixelShaderPath  If non-null, full pathname of the pixel shader file.
 */
Scene::Scene(char* imageFilePath, char* txtToolPath, char* outputPath, char* configFilePath, char* vertexShaderPath, char* pixelShaderPath)
{
	this->Init(imageFilePath, txtToolPath, outputPath, configFilePath, vertexShaderPath, pixelShaderPath);
}

/**
 * Initializes a scene, any images in the images directory (imageFilePath) 
 * will be converted to DDS format using the tool located in txtToolPath. 
 * Then the converted images (textures) are put into the outputPath folder. 
 * The other paths are self explanatory. After that, LoadScene() is called.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] imageFilePath    Directory where images to use are located
 * @param [in,out] txtToolPath		Directory where the DDS conversion tool is located.
 * @param [in,out] outputPath	    Directory where the converted images (textures) are located.
 * @param [in,out] configFilePath   Path where configuration file is located.
 * @param [in,out] vertexShaderPath Path where vertex shader file is located.
 * @param [in,out] pixelShaderPath  Path where pixel shader file is located.
 */
void Scene::Init(char* imageFilePath, char* txtToolPath, char* outputPath, char* configFilePath, char* vertexShaderPath, char* pixelShaderPath)
{
	//Convert images to DDS textures.
	int threadNum = 0;
	int lastIndexOfSlash = -1;
	char* currFolder;
	char* currCommand;
	wchar_t* finalOutputPath;
	wchar_t* textureRootPath;
	char* finalImagePath;
	char* commands[MAX_THREADS];
	HANDLE handles[MAX_THREADS];
	char* currOutputPath = NULL;
	char* currFileName = NULL;
	char* currFolderPath = NULL;
	char* currFolderAndFile = NULL;
	char* fileNameWithoutExt = NULL;
	char* fileNameWithTexturePath = NULL;
	vector<char*> needToLoadNames;

	mySceneModel = NULL;
	myCurrentImageIndex = 0;
	myConfigFilePath = configFilePath;
	myVertexShaderPath = vertexShaderPath;
	myPixelShaderPath = pixelShaderPath;

	if (imageFilePath != NULL)
	{
		//Go through each folder inside the images folder.
		myFolderNames = Ace::RetrieveFolderNames(imageFilePath);
		for (size_t currFolderIndex = 0; currFolderIndex < myFolderNames.size(); currFolderIndex++)
		{
			currFolderPath = Ace::ConcatenateStringsVA(3, imageFilePath, myFolderNames[currFolderIndex], "\\");
			myFileNames = Ace::RetrieveFileNames(currFolderPath);

			//Check to see what images have already been converted.
			for (size_t currFileNameIndex = 0; currFileNameIndex < myFileNames.size(); currFileNameIndex++)
			{
				currFileName = myFileNames[currFileNameIndex];

				//Construct the path of the corresponding .DDS file.
				fileNameWithoutExt = Ace::SubString(currFileName, 0, Ace::IndexOf(currFileName, "."));
				fileNameWithTexturePath = Ace::ConcatenateStringsVA(6, outputPath, "\\", myFolderNames[currFolderIndex] , "\\", fileNameWithoutExt, ".DDS");
				_aligned_free(fileNameWithoutExt);
				
				//Construct the path (the parent folder + currFileName) for the image to convert.
				currFolderAndFile = Ace::ConcatenateStringsVA(3, myFolderNames[currFolderIndex], "\\", currFileName);

				//Hold onto the file name without the ext for loading later on.
				myImageNames.push_back(fileNameWithTexturePath);

				//Add the name of the image if a .dds file has NOT been created for it already.
				if (Ace::DoesFileExist(fileNameWithTexturePath) == false)
				{
					needToLoadNames.push_back(currFolderAndFile);
				}
			}
		}

		//Create the texture directory if it does not exist.
		textureRootPath = Ace::ConvertStringToWCharString(outputPath);
		CreateDirectory(textureRootPath, NULL);
		_aligned_free(textureRootPath);

		//Execute texconv.exe to convert each of our images to DDS.
		//Take advantage of multi-threading to speed up conversion.
		for (size_t currImageIndex = 0; currImageIndex < needToLoadNames.size(); currImageIndex++)
		{
			//Create the corresponding directories inside the texture folder if they don't already exist.
			lastIndexOfSlash = Ace::LastIndexOf(needToLoadNames[currImageIndex], "\\");
			currFolder = Ace::SubString(needToLoadNames[currImageIndex], 0, lastIndexOfSlash);
			currOutputPath = Ace::ConcatenateStringsVA(3, outputPath, "\\", currFolder);
			finalImagePath = Ace::ConcatenateStringsVA(2, imageFilePath, needToLoadNames[currImageIndex]);
			
			//Need to convert the output path to wchar_t*.
			finalOutputPath = Ace::ConvertStringToWCharString(currOutputPath);
			CreateDirectory(finalOutputPath, NULL);

			//Create the command line command to send, don't use the wchar_t version of output path here.
			currCommand = Ace::ConcatenateStringsVA(5, txtToolPath, " -o ", currOutputPath, " -ft dds -m 1 ", finalImagePath);

			//Hold onto the commands so we can free them later.
			//We cant free them here because of thread timing it seems.
			commands[threadNum] = currCommand;

			_aligned_free(currFolder);
			_aligned_free(currOutputPath);
			_aligned_free(finalOutputPath);
			_aligned_free(finalImagePath);

			//Start the tread using the command line command we just created.
			//Store the thread handle so we can close it later.
			handles[threadNum++] = (HANDLE)_beginthreadex(NULL, 0, ExecuteCommand, currCommand, 0, 0);

			if (threadNum % MAX_THREADS == 0 || currImageIndex >= needToLoadNames.size() - 1)
			{
				//Wait for the other conversions to finish.
				WaitForMultipleObjects(threadNum, handles, true, INFINITE);
				for (int currHandleIndex = 0; currHandleIndex < threadNum; currHandleIndex++)
				{
					//Close all of the handles we waited for.
					CloseHandle(handles[currHandleIndex]);
					_aligned_free(commands[currHandleIndex]);
				}
				threadNum = 0;
			}
		}
	}

	if (myImageNames.size() > 0)
	{
		//Load the first image to initialize the scene.
		this->LoadScene(myImageNames[myCurrentImageIndex]);
	}
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Scene::~Scene()
{
	size_t currIndex = 0;

	if (mySceneModel != NULL)
	{
		delete mySceneModel;
		mySceneModel = NULL;
	}

	for (currIndex = 0; currIndex < myFileNames.size(); currIndex++)
	{
		_aligned_free(myFileNames[currIndex]);
	}

	for (currIndex = 0; currIndex < myFolderNames.size(); currIndex++)
	{
		_aligned_free(myFolderNames[currIndex]);
	}

	for (currIndex = 0; currIndex < myImageNames.size(); currIndex++)
	{
		_aligned_free(myImageNames[currIndex]);
	}
}

/**
 * I attempt to calculate the ideal height for the model based on the texture dimensions. These
 * percentage(s) are based on experimenting with the given images and determining what looked
 * best.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param panoramicType Type of the panoramic such as 180, 270, 360.
 * @param textureHeight Height of the texture.
 *
 * @return The calculated ideal panoramic height of the partial cylinder.
 */
float Scene::CalculateIdealPanoramicHeight(int panoramicType, float textureHeight)
{
	//Read in the percentage to use from the configuration file.
	char* idealRadiusPercentage = Scene::RetrieveConfigData(panoramicType, "idealHeightPercentage");
	float percentage = (float)atof(idealRadiusPercentage);

	_aligned_free(idealRadiusPercentage);
	return textureHeight * percentage;
}

/**
 * I attempt to calculate the ideal radius for the model based on the texture dimensions. These
 * percentage(s) are based on experimenting with the given images and determining what looked
 * best.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param panoramicType Type of the panoramic such as 180, 270, 360.
 * @param textureWidth  Width of the texture.
 *
 * @return The calculated ideal panoramic radius of the partial cylinder.
 */
float Scene::CalculateIdealPanoramicRadius(int panoramicType, float textureWidth)
{
	//Read in the percentage to use from the configuration file.
	char* idealRadiusPercentage = Scene::RetrieveConfigData(panoramicType, "idealRadiusPercentage");
	float percentage = (float)atof(idealRadiusPercentage);

	_aligned_free(idealRadiusPercentage);
	return textureWidth * percentage;
} 

/**
 * I attempt to calculate the ideal radius for the model based on the texture dimensions. These
 * percentages are based on experimenting with the given images and determining what looked best.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param textureWidth  Width of the texture.
 * @param textureHeight Height of the texture.
 *
 * @return The calculated ideal photosphere radius.
 */
float Scene::CalculateIdealPhotosphereRadius(float textureWidth, float textureHeight)
{
	//I use the part of the circle formula to get a combined number of the texture's dimensions.
	//Read in the percentage to use from the configuration file.
	float combinedNum = sqrtf((textureWidth * textureWidth) + (textureHeight * textureHeight));
	char* idealRadiusPercentage = Scene::RetrieveConfigData(999, "idealRadiusPercentage");
	float percentage = (float)atof(idealRadiusPercentage);

	_aligned_free(idealRadiusPercentage);
	return combinedNum * percentage;
}

/**
 * A function must have "__stdcall" if they are using Windows Multi-Threading. This function is
 * called by multiple threads to launch the .DDS converter tool.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] command If non-null, the command.
 *
 * @return 0 on success.
 */
unsigned int __stdcall Scene::ExecuteCommand(void* command)
{
	//Need to sanitize inputs so "Little Bobby Tables" doesn't shit on our chest.
	//i.e. Only allow commands that contain DDS somewhere in them.
	char* strCommand = static_cast<char*>(command);
	if (strCommand != NULL && Ace::ContainsIgnoreCase(strCommand, "DDS") == true)
	{
		return system(strCommand);
	}

	return -1;
}

/**
 * Loads next image by calling LoadScene().
 *
 * @author Katianie
 * @date 7/4/2016
 */
void Scene::LoadNextImage()
{
	//Prevent index from going out of bounds.
	if (myCurrentImageIndex >= myImageNames.size() - 1)
	{
		myCurrentImageIndex = 0;
	}
	else
	{
		//Increment to the next image.
		myCurrentImageIndex++;
	}

	//Get the image name and create the model to attach it too.
	this->LoadScene(myImageNames[myCurrentImageIndex]);
}

/**
 * Loads previous image by calling LoadScene().
 *
 * @author Katianie
 * @date 7/4/2016
 */
void Scene::LoadPreviousImage()
{
	//Prevent index from going out of bounds.
	if (myCurrentImageIndex <= 0)
	{
		myCurrentImageIndex = myImageNames.size() - 1;
	}
	else
	{
		//Decrement to the previous image.
		myCurrentImageIndex--;
	}

	//Get the image name and create the model to attach it too.
	this->LoadScene(myImageNames[myCurrentImageIndex]);
}

/**
 * Loads a scene; this is the "Meat and Potatoes" of the application. We use
 * the configuration file along with the texture path to load the textures and
 * attach them to the correct model. The models used are either a partial cylinder
 * depending on 180, 270 and 360, or they are a sphere for Photosphere. 
 * The configuration file is used to determine the ideal size and position of 
 * the camera and the model(s) based on size of the texture being loaded in. 
 * This allows for the user to use any image instead of just a select few.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] textureName If non-null, name of the texture.
 */
void Scene::LoadScene(char* textureName)
{
	Model* currModel = NULL;
	Material* currMaterial = NULL;
	Texture* currTexture = NULL;
	XMFLOAT3 modelPosition = XMFLOAT3(0.0, 0.0, 0.0);
	XMFLOAT4 modelRotation = XMFLOAT4(1.0, 0.0, 0.0, 0.0);
	char* strModelPosition = NULL;
	char* strModelRotation = NULL;
	char** tokens = NULL;
	float radius = 0.0;
	float height = 0.0;
	int panoramicType = 0;
	int numTokens = 0;
	TriangleSet currTriangleSet;

	if (textureName != NULL)
	{
		//Unload the previous model to save space.
		delete mySceneModel;

		//Construct the equivalent texture name and path using its image name.
		currTexture = new Texture(textureName);
		if (currTexture != NULL)
		{
			if (Ace::ContainsIgnoreCase(textureName, "panoramic") == true)
			{
				if (Ace::ContainsIgnoreCase(textureName, "180") == true)
				{
					//Half Panoramic Cylinder
					panoramicType = 180;
				}
				else if (Ace::ContainsIgnoreCase(textureName, "270") == true)
				{
					//75% Panoramic Cylinder
					panoramicType = 270;
				}
				else if (Ace::ContainsIgnoreCase(textureName, "360") == true)
				{
					//Panoramic Cylinder
					panoramicType = 360;
				}

				//Construct the cylinder model with the texture.
				radius = Scene::CalculateIdealPanoramicRadius(panoramicType, (float)currTexture->GetSizeWidth());
				height = Scene::CalculateIdealPanoramicHeight(panoramicType, (float)currTexture->GetSizeHeight());
				currTriangleSet.CreatePanoramic(panoramicType, radius, height);
			}
			else //Photosphere
			{
				panoramicType = 999;

				//Construct the sphere model with the texture.
				radius = CalculateIdealPhotosphereRadius((float)currTexture->GetSizeWidth(), (float)currTexture->GetSizeHeight());
				currTriangleSet.CreateSphere(radius);
			}

			//Read in configuration data, if none exists just use defaults.
			strModelPosition = Scene::RetrieveConfigData(panoramicType, "position");
			strModelRotation = Scene::RetrieveConfigData(panoramicType, "rotation");

			//Check if a position was provided in the configuration file.
			if (strModelPosition != NULL)
			{
				tokens = Ace::Tokenize(strModelPosition, ",", numTokens);

				modelPosition = XMFLOAT3((float)atof(tokens[0]),
					(float)atof(tokens[1]),
					(float)atof(tokens[2]));

				_aligned_free(strModelPosition);
			}

			//Check if a rotation was provided in the configuration file.
			if (strModelRotation != NULL)
			{
				tokens = Ace::Tokenize(strModelRotation, ",", numTokens);

				modelRotation = XMFLOAT4((float)atof(tokens[0]),
					(float)atof(tokens[1]),
					(float)atof(tokens[2]),
					(float)atof(tokens[3]));

				//Even if there is no rotation, we must specify an axis.
				if (modelRotation.x == 0.0f &&
					modelRotation.y == 0.0f &&
					modelRotation.z == 0.0f)
				{
					modelRotation = XMFLOAT4(1.0f, 0.0, 0.0, 0.0);
				}

				_aligned_free(strModelRotation);
			}

			if (tokens != NULL)
			{
				_aligned_free(tokens);
				tokens = NULL;
			}

			currMaterial = new Material(currTexture, myVertexShaderPath, myPixelShaderPath);
			mySceneModel = new Model(panoramicType, &currTriangleSet, modelPosition, modelRotation, currMaterial);
		}
	}
}

/**
 * Renders the current scene.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] projView MVP matrix.
 * @param red			    The red value of the back buffer.
 * @param green			    The green value of the back buffer.
 * @param blue			    The blue value of the back buffer.
 * @param alpha			    The alpha value of the back buffer.
 */
void Scene::Render(XMMATRIX* projView, float red, float green, float blue, float alpha)
{
	if(mySceneModel != NULL)
	{
		mySceneModel->Render(projView, red, green, blue, alpha);
	}
}

/**
 * Retrieves configuration data based on the panoramicType (180, 270, 360)
 * and the tag name (position, rotation).
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param panoramicType    Type of the panoramic (180, 270, 360).
 * @param [in,out] tagName The name of the tag (position, rotation) without the brackets.
 *
 * @return null if it fails, else a pointer to a char.
 */
char* Scene::RetrieveConfigData(int panoramicType, char* tagName)
{
	char* configFileData = Ace::RetrieveFileData(myConfigFilePath, true);
	char* currConfData = NULL;
	char* retVal = NULL;
	char* type = (char*)_aligned_malloc(128, BYTE_ALIGNMENT);
	char* tagOPEN = NULL;
	char* tagCLOSED = NULL;

	//If panoramicType is "999" then that means its a photosphere.
	if (panoramicType == 999)
	{
		strcpy_s(type, 12, "photosphere\0");
	}
	else
	{
		_itoa_s(panoramicType, type, 128, 10);
	}

	if (type != NULL && tagName != NULL)
	{
		//Get the settings corresponding to the specified panoramicType.
		tagOPEN = Ace::ConcatenateStringsVA(3, "<", type, ">");
		tagCLOSED = Ace::ConcatenateStringsVA(3, "</", type, ">");
		currConfData = Ace::RetrieveDataBetweenTags(configFileData, tagOPEN, tagCLOSED);
		_aligned_free(tagOPEN);
		_aligned_free(tagCLOSED);

		//Get the specific setting specified by the tagName.
		tagOPEN = Ace::ConcatenateStringsVA(3, "<", tagName, ">");
		tagCLOSED = Ace::ConcatenateStringsVA(3, "</", tagName, ">");
		retVal = Ace::RetrieveDataBetweenTags(currConfData, tagOPEN, tagCLOSED);
		_aligned_free(tagOPEN);
		_aligned_free(tagCLOSED);
		_aligned_free(currConfData);
	}

	_aligned_free(type);
	_aligned_free(configFileData);

	return retVal;
}

/**
 * Gets the current image index, 
 * used for the array of images.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The current image index.
 */
int Scene::GetCurrentImageIndex()
{
	return myCurrentImageIndex;
}

/**
 * Gets the scene model.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return the scene model.
 */
Model* Scene::GetSceneModel()
{
	return mySceneModel;
}

