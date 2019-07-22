#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void PlayerAssimililationObject::OnCreate()
	{
		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),//サイズ
			Vec3(0.0f, 0.0f, 0.0f),//原点
			Vec3(0.0f, XM_PI, 0.0f),//角度
			Vec3(0.0f, -0.7f, 0.0f)//位置
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