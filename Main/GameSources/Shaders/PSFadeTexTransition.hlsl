#include "INCStructs.hlsli"
#include "INCTransitionConstant.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSPCTInput input) : SV_TARGET
{
	float4 col = Color;

	float f = 1.0f - (Second.y / Second.x);
	float4 mask = g_texture.Sample(g_sampler, input.tex);
	col.a = 1.0f - (mask.r - (-1 + f * 2));

	return col;
}