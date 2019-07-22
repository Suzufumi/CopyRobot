#pragma once
#include "stdafx.h"

namespace basecross
{
	class TutorialMessage : public GameObject
	{
	public:
		struct InitParam
		{
			Vec3 m_StartPos;
			wstring m_TexKey;
		};

	private:
		InitParam m_InitParam;
		float m_TimeCount = 0.0f;
		float m_ButtonEnableTime;

	public:
		TutorialMessage(const shared_ptr<Stage>& stagePtr,TutorialMessage::InitParam initParam):
			GameObject(stagePtr),
			m_InitParam(initParam),
			m_ButtonEnableTime(1.0f){}
		virtual ~TutorialMessage() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;
		void Close();
	};
}