#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageSelectStage : public GameStage
	{
		shared_ptr<PostEffectRenderTarget> m_RenderTarget;
		shared_ptr<Transition> m_Transition;

		int m_StageNum;

		Vec3 m_CameraStartPos;
		vector<Vec3> m_CameraWays;
		int m_WayIndex;
		Vec3 m_StartPos;
		float m_TimeCount = 0.0f;
		Vec3 m_AtDir;

		virtual void CreateViewLight()override;
	public:
		StageSelectStage(int stageNum) :
			GameStage(),
			m_StageNum(stageNum),
			m_WayIndex(0)
		{}
		virtual ~StageSelectStage() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		shared_ptr<PostEffectRenderTarget> GetRenderTarget() { return m_RenderTarget; }
	};
}