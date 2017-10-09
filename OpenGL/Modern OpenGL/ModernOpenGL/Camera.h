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
#pragma once
#include "stdafx.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

class Camera
{
private:
	float myMoveSpeed;
	float myFieldOfView;
	float myAspectRatio;
	float myNearClipPlane;
	float myFarClipPlane;
	mat4 myTransformMatrix;
	vec3 myCameraPos;
	vec3 myCameraLookAtPoint;
	vec3 myCameraUpAxis;
	vec3 myBackgroundColor;

public:
	Camera();
	Camera(float moveSpeed, float fieldOfView, float aspectRatio, float nearClipPlane, float farClipPlane, vec3 cameraPos, vec3 cameraLookAtPoint, vec3 cameraUpAxis, vec3 backgroundColor);
	virtual ~Camera()
	{

	}

	//Getters
	float getMoveSpeed();
	float getFieldOfView();
	float getAspectRatio();
	float getNearClipPlane();
	float getFarClipPlane();
	mat4 getTransformMatrix();
	vec3 getCameraPos();
	vec3 getCameraLookAtPoint();
	vec3 getCameraUpAxis();
	vec3 getBackgroundColor();
	
	//Setters
	void setMoveSpeed(float speed);
	void setFieldOfView(float fieldOfView);
	void setAspectRatio(float aspectRatio);
	void setNearClipPlane(float nearClipPlane);
	void setFarClipPlane(float farClipPlane);
	void setTransformMatrix(const mat4& transform);
	void setCameraPos(vec3 cameraPos);
	void setCameraLookAtPoint(vec3 cameraLookAtPoint);
	void setCameraUpAxis(vec3 cameraUpAxis);
	void setBackgroundColor(vec3 backgroundColor);

	//Functions
	//Creates the combined ModelViewProjection matrix and passes it to the shader.
	mat4 calculateMVPMatrix(const mat4& modelMatrix);

};