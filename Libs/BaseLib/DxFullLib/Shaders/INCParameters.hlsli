
cbuffer ParametersBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4x4 InvMatrix : packoffset(c12);
	float4 Emissive : packoffset(c16);
	float4 Diffuse : packoffset(c17);
	float4 Specular : packoffset(c18);
	uint4 Activeflags : packoffset(c19);			//ƒtƒ‰ƒO
	float4 LightDir	: packoffset(c20);
	float4 LightPos	: packoffset(c21);
	float4 EyePos	: packoffset(c22);
	float4 EyeDir   : packoffset(c23);
	float4x4 LightView	: packoffset(c24);
	float4x4 LightProjection	: packoffset(c28);
	float4x3 Bones[72] : packoffset(c32);
};
