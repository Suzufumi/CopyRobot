#pragma once
#include "stdafx.h"

namespace basecross
{
	class AliveGameObject : public OurGameObject
	{
	public:
		AliveGameObject(const shared_ptr<Stage>& stagePtr)
			: OurGameObject(stagePtr)
		{}
		virtual ~AliveGameObject()
		{}
		// �\�z
		virtual void OnCreate() override
		{}
		// �X�V
		virtual void OnUpdate() override
		{}
		// �ړ�
		virtual void Move(Vec3 direction)
		{}
		// �������~�߂�
		virtual void Stop() override 
		{}
	};
}
//end basecross