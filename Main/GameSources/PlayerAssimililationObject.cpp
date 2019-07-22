#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void PlayerAssimililationObject::OnCreate()
	{
		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),//�T�C�Y
			Vec3(0.0f, 0.0f, 0.0f),//���_
			Vec3(0.0f, XM_PI, 0.0f),//�p�x
			Vec3(0.0f, -0.7f, 0.0f)//�ʒu
		);

		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"ENEMY_MESH");
		SetAlphaActive(true);
		drawComp->SetBlendState(BlendState::AlphaBlend);
		drawComp->SetDepthStencilState(DepthStencilState::Default);
		drawComp->SetRasterizerState(RasterizerState::DoubleDraw);
		drawComp->BoneInit();
		ResetDiffuse();
		drawComp->SetMeshToTransformMatrix(spanMat);

		SetDrawLayer(-1);
	}

	void PlayerAssimililationObject::OnUpdate()
	{

	}

	void PlayerAssimililationObject::ResetDiffuse()
	{
		GetDynamicComponent<SmBaseDraw>()->SetDiffuse(Col4(0.4f, 0.4f, 0.4f, 0.0f));
	}

	void PlayerAssimililationObject::ChangeAlpha(float alpha)
	{
		auto drawComp = GetDynamicComponent<SmBaseDraw>();
		Col4 color = drawComp->GetDiffuse();
		color.w = alpha;
		drawComp->SetDiffuse(color);
	}

	void PlayerAssimililationObject::ChangeMeshResource(wstring key)
	{
		GetDynamicComponent<SmBaseDraw>()->SetMeshResource(key);
	}
}