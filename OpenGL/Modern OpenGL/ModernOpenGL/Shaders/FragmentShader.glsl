#version 120

//Interpolated values from the vertex shader.
varying vec4 fragmentColor;
varying vec2 UV;

uniform sampler2D textureSampler;

void main()
{
	gl_FragColor = texture2D( textureSampler, UV ) + fragmentColor;
}