cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType{
	float4 mPosition : POSITION;
	float4 mColor : COLOR;
};

struct PixelInputType{
	float4 mPosition : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType input){
	PixelInputType output;
	input.mPosition.w = 1.0f;
	
	output.mPosition = mul(input.mPosition, worldMatrix);
	output.mPosition = mul(output.mPosition, viewMatrix);
	output.mPosition = mul(output.mPosition, projectionMatrix);
	
	output.mColor = input.mColor;
	
	return output;
}