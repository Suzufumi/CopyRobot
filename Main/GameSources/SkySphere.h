#pragma once
#include "stdafx.h"

namespace basecross
{
	class SkySphere : public GameObject
	{
	public:
		SkySphere(const shared_ptr<Stage>& ptrStage) :
			GameObject(ptrStage) {}
		virtual ~SkySphere() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}