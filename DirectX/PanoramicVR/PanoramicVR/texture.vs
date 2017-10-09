///Variables
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer Vertex
{
	float4 firstPos;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	bool clipBack : PSIZE0;
};

///Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    
	//Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	
	//Prevent the back of the model from drawing a distorted repeated texture.
	//This is because 180 and 270 are not complete geometry by natrue of being 
	//half or 3/4 of a cylinder.
	if(input.position.z >= firstPos.z)
	{
		output.clipBack = true;
	}
	else
	{
		output.clipBack = false;
	}
	
	//Dont clip the back if the geometry is not 180 or 270.
	if(firstPos.z >= 999.9f)
	{
		output.clipBack = false;
	}
	
	//Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	
	//Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
    return output;
}