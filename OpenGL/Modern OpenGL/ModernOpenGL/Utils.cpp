/** Modern OpenGL
* This program is an example of how to use OpenGL after 2015.
* Many things have changed over the years including libraries,
* and now built in features. This program takes advantage of 
* Vertex Buffer Processing, Shaders, GLEW, GLFW, and more.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2015 Katianie.com
*/
#include "Utils.h"

//Displays provided error message to stdout and destroys the window if not null.
void Utils::displayErrorAndExit(GLFWwindow* window, string errorString)
{
	printf("%s\n\0", errorString);

	//If the window is created then destroy it.
	if (window != NULL)
	{
		glfwDestroyWindow(window);
	}

	glfwTerminate();

	system("pause"); //Allow user to see the error before closing.
	exit(-1);		 //Close the program with error.
}

char* Utils::readFile(GLFWwindow* window, const char* filepath)
{
	FILE* theFile;
	size_t readResult;
	int sizeOfBuffer;
	long sizeOfFile;
	char* buffer;
	string errorString;

	//Attempt to open the specified file.
	if (fopen_s(&theFile, filepath, "rb") != 0)
	{
		errorString = "File Error: Could not open" + string(filepath);
		displayErrorAndExit(window, errorString);
		return NULL;
	}

	//Obtain file size:
	fseek(theFile, 0, SEEK_END);
	sizeOfFile = ftell(theFile);
	rewind(theFile);

	//Allocate memory to contain the whole file (plus a null terminator).
	sizeOfBuffer = sizeof(char) * (sizeOfFile + 1);
	buffer = (char*)calloc(sizeOfFile + 1, sizeof(char));
	if (buffer == NULL)
	{ 
		errorString = "File Error: Could not allocate " + to_string(sizeOfBuffer) + " bytes.";
		displayErrorAndExit(window, errorString);
		return NULL;
	}

	//Copy the file into the buffer:
	readResult = fread(buffer, 1, sizeOfFile, theFile);
	if (readResult != sizeOfFile)
	{ 
		errorString = "File Error:" + string(filepath) + " could not be read.";
		displayErrorAndExit(window, errorString);
		return NULL;
	}

	//Cleanup
	fclose(theFile);

	//Return the data just read.
	return buffer;
}

GLuint Utils::loadImageDDS(GLFWwindow* window, const char* imagepath)
{
	unsigned char* imageBuffer;
	unsigned char header[124];
	unsigned int imageBuffSize;
	unsigned int imageHeight;
	unsigned int imageWidth;
	unsigned int linearSize;
	unsigned int mipMapCount;
	unsigned int fourCC;
	unsigned int imageComponents;
	unsigned int imageFormat;
	unsigned int imageBlockSize;
	unsigned int imageOffset = 0;
	unsigned int currLevel;
	unsigned int currSize;
	char filecode[4];
	FILE* imageFile;
	GLuint textureID;
	
	//Attempt to open the image file.
	if (fopen_s(&imageFile, imagepath, "rb") != 0)
	{
		displayErrorAndExit(window, "Error: DDS Image could not be found.");
		return 0;
	}

	//Verify the type of file.
	fread(filecode, 1, 4, imageFile);
	if (strncmp(filecode, "DDS ", 4) != 0) 
	{
		//If the file is not DDS, close the file, display error, and close.
		fclose(imageFile);
		displayErrorAndExit(window, "Error: Image file is not DDS format.");
		return 0;
	}

	//Get the surface desc.
	fread(&header, 124, 1, imageFile);

	//Retrieve the image's properties.
	imageHeight = *(unsigned int*)&(header[8]);
	imageWidth = *(unsigned int*)&(header[12]);
	linearSize = *(unsigned int*)&(header[16]);
	mipMapCount = *(unsigned int*)&(header[24]);
	fourCC = *(unsigned int*)&(header[80]);

	//How big is it going to be including all mipmaps?
	if (mipMapCount > 1)
	{
		imageBuffSize = linearSize * 2;
	}
	else
	{
		imageBuffSize = linearSize;
	}

	//Read the bulk of the image data.
	imageBuffer = (unsigned char*)malloc(imageBuffSize * sizeof(unsigned char));
	fread(imageBuffer, 1, imageBuffSize, imageFile);
	
	//Close the file since we read all the data.
	fclose(imageFile);

	//Determine the type of comperssion used.
	if (fourCC == FOURCC_DXT1)
	{
		imageComponents = 3;
	}
	else
	{
		imageComponents = 4;
	}
	switch (fourCC)
	{
		case FOURCC_DXT1:
			imageFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			imageFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			imageFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(imageBuffer);
			displayErrorAndExit(window, "Error Image file is not in valid compression format.");
			return 0;
	}

	//Create one OpenGL texture
	glGenTextures(1, &textureID);

	//"Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Determine the size of each compressed block.
	if (imageFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		imageBlockSize = 8;
	}
	else
	{
		imageBlockSize = 16;
	}

	//Load the mipmaps.
	for (currLevel = 0; currLevel < mipMapCount && (imageWidth || imageHeight); ++currLevel)
	{
		currSize = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * imageBlockSize;

		glCompressedTexImage2D(GL_TEXTURE_2D, currLevel, imageFormat, imageWidth, imageHeight, 0, currSize, imageBuffer + imageOffset);

		imageOffset += currSize;
		imageWidth /= 2;
		imageHeight /= 2;

		//Handle Non-Power-Of-Two textures.
		if (imageWidth < 1)
		{
			imageWidth = 1;
		}
		if (imageHeight < 1)
		{
			imageHeight = 1;
		}

	}

	free(imageBuffer);

	return textureID;


}