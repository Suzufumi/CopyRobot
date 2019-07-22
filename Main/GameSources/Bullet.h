#pragma once
#include "stdafx.h"

namespace basecross
{
	class Bullet : public GameObject
	{
	public:
		struct InitializeParam
		{
			Vec3 m_Position;
			Vec3 m_Velocity;
		};

	protected:
		InitializeParam m_InitParam;

	public:
		Bullet(const shared_ptr<Stage>& ptrStage, Bullet::InitializeParam initParam);
		virtual ~Bullet(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec)override;
	};
}