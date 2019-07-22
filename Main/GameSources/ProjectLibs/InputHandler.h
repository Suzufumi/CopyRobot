#pragma once
#include "stdafx.h"

namespace basecross
{

	class InputHandler
	{
	public:
		enum StickDownDir
		{
			NEUTRAL, UP, RIGHT, DOWN, LEFT
		};

		enum PressType
		{
			PRESSED,RELEASED,PRESS
		};

	private:
		map<int, std::function<void()>> m_PressedFuncMap,m_ReleasedFuncMap,m_PressFuncMap;
		std::function<void(const StickDownDir stickDownDir)> m_LStickDownFunc;
		vector<std::pair<int, PressType>> m_WaitUnregister;

		Vec2 m_LStickDir;
		Vec2 m_RStickDir;

		float m_LTrigger;
		float m_RTrigger;

		bool m_IsStickDown;

		bool CheckRegistered(const int buttonName,const map<int, std::function<void()>>& funcMap)const;
		bool Unregister(const int buttonName,map<int, std::function<void()>>& funcMap);
		bool UnregisterEach(const int buttonName, const PressType pressType);


	public:
		InputHandler() :
			m_IsStickDown(false)
		{}
		virtual ~InputHandler() {}

		void RegisterInputFunction(const int buttonName,const std::function<void()>& func,const PressType pressType);
		void RegisterLStickDownFunction(const std::function<void(const StickDownDir stickDownDir)>& func);
		bool UnregisterInputFunction(const int buttonName, const PressType pressType);

		bool IsRegistered(const int buttonName, const PressType pressType) const;

		void PushHandle();
		void StickHandle(const float minStickLength = 0.1f);
		void TriggerHandle(const float minTriggerLength = 0.1f);
		void LStickDownHandle();

		Vec2 GetLStickDir()const { return m_LStickDir; }
		Vec2 GetRStickDir()const { return m_RStickDir; }
		float GetLTriggerValue()const { return m_LTrigger; }
		float GetRTriggerValue()const { return m_RTrigger; }
	};

}