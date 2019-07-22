//--------------------------------------------------------------------------------------
// File: PSPVTStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

Texture2D g_texture : register(t0);
Texture2D g_lineTex : register(t1);
SamplerState g_sampler : register(s0);

float4 main(PSPNTInput input) : SV_TARGET
{
	float dif = clamp(dot(input.norm,-LightDir), 0.0, 1.0);
	if (!Activeflags.z)
		dif = 1.0f;

	float3 R = normalize(LightDir.xyz - input.norm * 2 * dot(LightDir.xyz, input.norm));
	float spec = pow(clamp(dot(R, -EyeDir), 0.0, 1.0), Specular.w);
	if (Specular.w <= 0.0)
		spec = 0.0;

	float4 color = Diffuse * float4(dif, dif, dif, 1.0f) + Specular * float4(spec,spec,spec,0.0f) + Emissive;
	float4 Light = color;

	if (Activeflags.x) {
		Light = g_texture.Sample(g_sampler, input.tex) * Light;
	}

	if (Activeflags.y) {
		float4 lineCol = g_lineTex.Sample(g_sampler, input.tex);
		if (lineCol.a > 0.01)
		{
			if (lineCol.a < 1.0)
			{
				lineCol.rgb = Light * (1.0 - lineCol.a) + lineCol.rgb * lineCol.a;
			}
			lineCol.a = Diffuse.a;
			Light = lineCol;
		}
	}

	return Light;
}
