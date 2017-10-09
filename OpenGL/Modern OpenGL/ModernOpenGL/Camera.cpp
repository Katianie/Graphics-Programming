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
#include "Camera.h"

Camera::Camera()
{
	myMoveSpeed = 0.1;
	myFieldOfView = 45.0f;
	myAspectRatio = 4.0f / 3.0f;
	myNearClipPlane = 0.01f;
	myFarClipPlane = 100000.0f;
	myTransformMatrix = mat4(1.0f);
	myCameraPos = vec3(0, 0, 6);
	myCameraLookAtPoint = vec3(0, 0, 0);
	myCameraUpAxis = vec3(0, 1, 0);
	myBackgroundColor = vec3(0, 0, 0);

	glClearColor(myBackgroundColor.x, myBackgroundColor.y, myBackgroundColor.z, 1.0f);
}

Camera::Camera(float moveSpeed, float fieldOfView, float aspectRatio, float nearClipPlane, float farClipPlane, vec3 cameraPos, vec3 cameraLookAtPoint, vec3 cameraUpAxis, vec3 backgroundColor)
{
	myMoveSpeed = moveSpeed;
	myFieldOfView = fieldOfView;
	myAspectRatio = aspectRatio;
	myNearClipPlane = nearClipPlane;
	myFarClipPlane = farClipPlane;
	myTransformMatrix = mat4(1.0f);
	myCameraPos = cameraPos;
	myCameraLookAtPoint = cameraLookAtPoint;
	myCameraUpAxis = cameraUpAxis;
	myBackgroundColor = backgroundColor;

	glClearColor(myBackgroundColor.x, myBackgroundColor.y, myBackgroundColor.z, 1.0f);
}

//Getters
float Camera::getMoveSpeed()
{
	return myMoveSpeed;
}

float Camera::getFieldOfView()
{
	return myFieldOfView;
}

float Camera::getAspectRatio()
{
	return myAspectRatio;
}

float Camera::getNearClipPlane()
{
	return myNearClipPlane;
}

float Camera::getFarClipPlane()
{
	return myFarClipPlane;
}

mat4 Camera::getTransformMatrix()
{
	return myTransformMatrix;
}

vec3 Camera::getCameraPos()
{
	return myCameraPos;
}

vec3 Camera::getCameraLookAtPoint()
{
	return myCameraLookAtPoint;
}

vec3 Camera::getCameraUpAxis()
{
	return myCameraUpAxis;
}

vec3 Camera::getBackgroundColor()
{
	return myBackgroundColor;
}

//Setters
void Camera::setMoveSpeed(float moveSpeed)
{
	myMoveSpeed = moveSpeed;
}

void Camera::setFieldOfView(float fieldOfView)
{
	myFieldOfView = fieldOfView;
}

void Camera::setAspectRatio(float aspectRatio)
{
	myAspectRatio = aspectRatio;
}

void Camera::setNearClipPlane(float nearClipPlane)
{
	myNearClipPlane = nearClipPlane;
}

void Camera::setFarClipPlane(float farClipPlane)
{
	myFarClipPlane = farClipPlane;
}

void Camera::setTransformMatrix(const mat4& transform)
{
	myTransformMatrix = transform;
}

void Camera::setCameraPos(vec3 cameraPos)
{
	myCameraPos = cameraPos;
}

void Camera::setCameraLookAtPoint(vec3 cameraLookAtPoint)
{
	myCameraLookAtPoint = cameraLookAtPoint;
}

void Camera::setCameraUpAxis(vec3 cameraUpAxis)
{
	myCameraUpAxis = cameraUpAxis;
}

void Camera::setBackgroundColor(vec3 backgroundColor)
{
	myBackgroundColor = backgroundColor;
	glClearColor(myBackgroundColor.x, myBackgroundColor.y, myBackgroundColor.z, 1.0f);
}

//Functions
//Creates the combined ModelViewProjection matrix and passes it to the shader.
mat4 Camera::calculateMVPMatrix(const mat4& modelMatrix)
{
	mat4 perspectiveProjection;
	mat4 cameraView;

	//Maps our 3D world (with perspective projection) to the 2D viewport aka the screen.
	perspectiveProjection = perspective(myFieldOfView, myAspectRatio, myNearClipPlane, myFarClipPlane);

	//Creates our view baised off of a camera and perspective projection.
	cameraView = lookAt(myCameraPos, myCameraLookAtPoint, myCameraUpAxis);

	//Our ModelViewProjection : multiplication of our 3 matrices
	return perspectiveProjection * cameraView * myTransformMatrix * modelMatrix;
}