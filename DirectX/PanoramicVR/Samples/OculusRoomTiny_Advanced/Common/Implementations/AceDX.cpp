/************************************************************************************
Filename		:   AceDX.cpp
Content			:	Implementation of the AceDX class.
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
#include "..//Headers//stdafx.h"
#include "..//Headers//AceDX.h"

/**
 * Converts OVR Matrix to a XMMatrix.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param mat The matrix to convert.
 *
 * @return The OVR converted matrix to XMMatrix.
 */
XMMATRIX AceDX::ConvertOVRMatrixToXMMatrix(ovrMatrix4f mat)
{
	return(	XMMatrixSet(mat.M[0][0], mat.M[1][0], mat.M[2][0], mat.M[3][0],
						mat.M[0][1], mat.M[1][1], mat.M[2][1], mat.M[3][1],
						mat.M[0][2], mat.M[1][2], mat.M[2][2], mat.M[3][2],
						mat.M[0][3], mat.M[1][3], mat.M[2][3], mat.M[3][3]));
}

/**
 * Convert OVR quaternion to XMFloat4.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param quat The quaternion to convert.
 *
 * @return The OVR converted quaternion to XMFloat4.
 */
XMFLOAT4 AceDX::ConvertOVRQuaternionToXMFloat4(ovrQuatf quat)
{
	return(XMFLOAT4(quat.x, quat.y, quat.z, quat.w));
}

/**
 * Convert OVR quaternion to XMVector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param quat The quaternion to convert.
 *
 * @return The OVR converted quaternion to XMVector.
 */
XMVECTOR AceDX::ConvertOVRQuaternionToXMVector(ovrQuatf quat)
{ 
	return(XMVectorSet(quat.x, quat.y, quat.z, quat.w));
}

/**
 * Convert OVRVector3 to XMFloat3.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vect The vector to convert.
 *
 * @return The OVR converted vector 3 to XMFloat3.
 */
XMFLOAT3 AceDX::ConvertOVRVector3ToXMFloat3(ovrVector3f vect)
{
	return(XMFLOAT3(vect.x, vect.y, vect.z));
}

/**
 * Convert OVRVector3 to XMVector.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param vect The vector to convert.
 *
 * @return The OVR converted vector 3 to XM vector.
 */
XMVECTOR AceDX::ConvertOVRVector3ToXMVector(ovrVector3f vect)
{ 
	return(XMVectorSet(vect.x, vect.y, vect.z, 0));
}

/**
 * Convert string to XMFloat3.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] str If non-null, the string to convert.
 *
 * @return The string converted to XMFloat3.
 */
XMFLOAT3 AceDX::ConvertStringToXMFloat3(char* str)
{
	int numComps = 0;
	float* comps = Ace::ConvertStringToFloatArray(str, ",", numComps);
	XMFLOAT3 retVal(comps[0], comps[1], comps[2]);

	delete comps;
	return retVal;
}

/**
 * Convert string to XMFloat4.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] str If non-null, the string to convert.
 *
 * @return The string converted to XMFloat4.
 */
XMFLOAT4 AceDX::ConvertStringToXMFloat4(char* str)
{
	int numComps = 0;
	float* comps = Ace::ConvertStringToFloatArray(str, ",", numComps);
	XMFLOAT4 retVal(comps[0], comps[1], comps[2], comps[3]);
	
	delete comps;
	return retVal;
}

/**
 * Ensure an expression evaluates to true, otherwise display an error message.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param expression The expression to test if true or false.
 * @param [in] msgIfFail If non-null, the message to display if false.
 */
void AceDX::Validate(bool expression, char* msgIfFail)
{
	if (expression == false)
	{
		MessageBoxA(NULL, msgIfFail, "PanoramicVR", MB_ICONERROR | MB_OK);
		exit(-1);
	}
}