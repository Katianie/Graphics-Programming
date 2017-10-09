#version 120

//Input vertex and color data, different for all executions of this shader.
attribute vec3 vertexPosInModelSpace;
attribute vec3 vertexColor;
attribute vec2 vertexUV;

//Output data ; will be interpolated for each fragment.
varying vec4 fragmentColor;
varying vec2 UV;

//Input of the combined perspective projection, camera view, and model space.
uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosInModelSpace, 1);

	fragmentColor = vec4(vertexColor, 1);
	UV = vertexUV;
}