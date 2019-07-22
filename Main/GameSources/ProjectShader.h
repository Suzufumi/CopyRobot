/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	struct TransitionConstants
	{
		Vec4 Second;
		Col4 Color;
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBTransition, TransitionConstants)

	DECLARE_DX11_VERTEX_SHADER(VSTransition, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSTransition)
	DECLARE_DX11_PIXEL_SHADER(PSFadeTexTransition);

	DECLARE_DX11_PIXEL_SHADER(PSPCTGrayScaleSprite)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSepiaSprite)
}
//end basecross

