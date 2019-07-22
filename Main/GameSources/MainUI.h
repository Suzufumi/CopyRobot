#pragma once
#include "stdafx.h"

namespace basecross
{
	class Gauge : public Sprite
	{
		float m_Value;
		Vec2 m_StartPos;

		shared_ptr<Sprite> m_GaugeBase;
	public:
		Gauge(const shared_ptr<Stage>& stagePtr,Vec2 position);
		virtual ~Gauge() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		virtual void AdjustVertex()override;
		virtual void UpdateVertex(float elapsedTime, VertexPositionColorTexture* vertices)override;

		float GetValue() { return m_Value; }
		void SetValue(float value) { m_Value = value; }
	};
}
