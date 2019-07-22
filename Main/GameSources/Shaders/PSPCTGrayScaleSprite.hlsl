#include "INCStructs.hlsli"


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};

float4 main(PSPCTInput input) : SV_TARGET
{
	float3 grayScale = float3(0.298912f,0.586611f,0.114478f);
	float gray = dot(grayScale, (float3)g_texture.Sample(g_sampler, input.tex));

	//テクスチャとカラーから仮想ライティングを作成
	float4 Light = float4(gray,gray,gray,Diffuse.a) * input.color;
	//エミッシブを足す
	return saturate(Light * Diffuse + Emissive);
}