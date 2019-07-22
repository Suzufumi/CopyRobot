#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bullet::Bullet(const shared_ptr<Stage>& stagePtr, Bullet::InitializeParam initParam) :
		GameObject(stagePtr),
		m_InitParam(initParam)
	{

	}

	void Bullet::OnCreate()
	{
		auto trans = AddComponent<Transform>();
		trans->SetPosition(m_InitParam.m_Position);

		auto rotToHead = GetBehavior<UtilBehavior>();
		rotToHead->RotToHead(m_InitParam.m_Velocity, 1.0f);

		auto rigid = AddComponent<Rigidbody>();
		rigid->SetVelocity(m_InitParam.m_Velocity);

		auto collision = AddComponent<CollisionObb>();
		collision->SetIsHitAction(IsHitAction::None);
		collision->AddExcludeCollisionTag(L"Enemy");

		auto drawComp = AddComponent<PNTStaticModelDraw>();
		drawComp->SetMeshResource(L"BULLET_MESH");
		drawComp->SetLineTextureResource(L"BULLET_TX");

		AddTag(L"Bullet");
	}

	void Bullet::OnUpdate()
	{

	}

	void Bullet::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto& Obj : OtherVec)
		{
			if (Obj->FindTag(L"Wall"))
			{
				GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			}
		}
	}
}