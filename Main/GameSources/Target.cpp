#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TargetCursor::TargetCursor(const shared_ptr<Stage>& ptrStage) :
		GameObject(ptrStage),
		m_PositionDiff(Vec3(0,1.5f,0))
	{

	}

	void TargetCursor::OnCreate()
	{
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"TARGET_MESH");
		drawComp->SetLineTextureResource(L"TARGET_TX");
		drawComp->AddAnimation(L"DEFAULT", 0, 119, true, 60);
		drawComp->ChangeCurrentAnimation(L"DEFAULT");

		SetAlphaActive(true);
	}

	void TargetCursor::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);
	}

	void TargetCursor::SetTarget(const shared_ptr<GameObject>& target)
	{
		m_Target = target;

		vector<VertexPositionNormalTextureSkinning> vertices = target->GetDynamicComponent<SmBaseDraw>()->GetMeshResource()->GetBackupVerteces<VertexPositionNormalTextureSkinning>();
		std::sort(vertices.begin(), vertices.end(), [](VertexPositionNormalTextureSkinning a, VertexPositionNormalTextureSkinning b) {
			return a.position.y > b.position.y;
		});

		m_TargetTopVertex = vertices[0];

		vector<Mat4x4> bones = target->GetDynamicComponent<SmBaseDraw>()->GetVecLocalBones();
		Mat4x4 skinning(Flt4(0,0,0,0), Flt4(0, 0, 0, 0), Flt4(0, 0, 0, 0), Flt4(0, 0, 0, 0));
		for (int i = 0; i < 4; i++)
		{
			skinning += bones[m_TargetTopVertex.indices[i]] * m_TargetTopVertex.weights[i];
		}

		Vec4 pos(m_TargetTopVertex.position.x, m_TargetTopVertex.position.y, m_TargetTopVertex.position.z, 1.0f);
		
		Vec3 vertPos = pos * skinning;

		Vec3 setPos = m_Target->GetComponent<Transform>()->GetPosition();
		setPos.y = vertPos.y;
		GetComponent<Transform>()->SetPosition(setPos);
	}
}