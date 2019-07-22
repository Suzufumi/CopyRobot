#include "INCStructs.hlsli"
#include "INCTransitionConstant.hlsli"

float4 main(PSPCTInput input) : SV_TARGET
{
	float4 col = Color;

	float f = 1.0f -  (Second.y / Second.x);
	col.a = f;

	return col;
}