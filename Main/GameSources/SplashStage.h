#pragma once
#include "stdafx.h"

namespace basecross
{
	class SplashStage :public Stage
	{
		vector<wstring> m_DisplayTex;
		int m_Index = 0;

		weak_ptr<GameObject> m_Sprite;

		float m_TimeCount = 0.0f;
		float m_Interval = 3.0f;

		void CreateViewLight();

	public: SplashStage() : Stage() {}
			virtual ~SplashStage() {}
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
	};

}