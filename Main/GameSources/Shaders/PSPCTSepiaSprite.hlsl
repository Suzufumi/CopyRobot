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
	
	float4 tex = g_texture.Sample(g_sampler, input.tex);
	float4 c;
	c.r = tex.r*0.393f + tex.g*0.769f + tex.b*0.189f;
	c.g = tex.r*0.349f + tex.g*0.686f + tex.b*0.168f;
	c.b = tex.r*0.272f + tex.g*0.534f + tex.b*0.131f;
	c.a = tex.a;

	//テクスチャとカラーから仮想ライティングを作成
	float4 Light = c * input.color;
	//エミッシブを足す
	return saturate(Light * Diffuse + Emissive);
}