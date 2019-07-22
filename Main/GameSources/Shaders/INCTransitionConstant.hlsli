cbuffer TransitionConstantBuffer : register(b0)
{
	float4 Second : packoffset(c0);
	float4 Color : packoffset(c1);
}