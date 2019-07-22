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
		// 構築
		virtual void OnCreate() override
		{}
		// 更新
		virtual void OnUpdate() override
		{}
		// 移動
		virtual void Move(Vec3 direction)
		{}
		// 動きを止める
		virtual void Stop() override 
		{}
	};
}
//end basecross