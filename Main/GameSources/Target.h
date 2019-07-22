#pragma once
#include "stdafx.h"

namespace basecross
{
	class TargetCursor : public GameObject
	{
		shared_ptr<GameObject> m_Target;
		VertexPositionNormalTextureSkinning m_TargetTopVertex;
		Vec3 m_PositionDiff;
	public:
		TargetCursor(const shared_ptr<Stage>& ptrStage);
		~TargetCursor(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetTarget(const shared_ptr<GameObject>& target);
	};
}