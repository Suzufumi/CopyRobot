/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBTransition)

	IMPLEMENT_DX11_VERTEX_SHADER(VSTransition, App::GetApp()->GetShadersPath() + L"VSTransition.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSTransition, App::GetApp()->GetShadersPath() + L"PSNormalTransition.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSFadeTexTransition, App::GetApp()->GetShadersPath() + L"PSFadeTexTransition.cso")

	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTGrayScaleSprite, App::GetApp()->GetShadersPath() + L"PSPCTGrayScaleSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSepiaSprite, App::GetApp()->GetShadersPath() + L"PSPCTSepiaSprite.cso")
}
//end basecross


