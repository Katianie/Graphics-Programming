/************************************************************************************
Filename		:   Camera.cpp
Content			:	Implementation of the Camera class.
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
#include "../Headers/Camera.h"

/**
 * Default Constructor
 *
 * @author Katianie
 * @date 7/4/2016
 */
Camera::Camera()
{
	Init(XMVECTOR(), XMVECTOR());
}

/**
 * Parameterized Constructor.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in] position If non-null, the position.
 * @param [in] rotation If non-null, the rotation.
 */
Camera::Camera(XMVECTOR* position, XMVECTOR* rotation)
{
	Init(*position, *rotation);
}

/**
 * Initialized the instance variables of the Camera.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param position The position.
 * @param rotation The rotation.
 */
void Camera::Init(XMVECTOR position, XMVECTOR rotation)
{
	myPosition = position;
	myRotation = rotation;
}

/**
 * Destructor.
 *
 * @author Katianie
 * @date 7/4/2016
 */
Camera::~Camera()
{
	//Nothing to delete.
}

/**
 * Gets the X,Y,Z Position of the Camera.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The X,Y,Z Position of the Camera.
 */
XMVECTOR Camera::GetPosition()
{
	return myPosition;
}

/**
 * Gets the rotation vector of the camera.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The rotation vector of the camera.
 */
XMVECTOR Camera::GetRotation()
{
	return myRotation;
}

/**
 * Gets view matrix.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @return The view matrix.
 */
XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, -1, 0), myRotation);
	return(XMMatrixLookAtRH(myPosition, XMVectorAdd(myPosition, forward), XMVector3Rotate(XMVectorSet(0, 1, 0, 0), myRotation)));
}

/**
 * Sets the X,Y,Z Position of the Camera.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param pos The the X,Y,Z Position of the Camera.
 */
void Camera::SetPosition(XMVECTOR pos)
{
	myPosition = pos;
}

/**
 * Sets the rotation vector of the camera.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param rot The rotation vector of the camera.
 */
void Camera::SetRotation(XMVECTOR rot)
{
	myRotation = rot;
}
