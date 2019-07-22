#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	bool InputHandler::CheckRegistered(const int buttonName,const map<int, std::function<void()>>& funcMap)const
	{
		auto it = funcMap.find(buttonName);
		if (it != funcMap.end())
		{
			return true;
		}

		return false;
	}

	bool InputHandler::Unregister(const int buttonName, map<int, std::function<void()>>& funcMap)
	{
		auto it = funcMap.find(buttonName);
		if (it != funcMap.end())
		{
			funcMap.erase(it);
		}
		else
		{
			return false;
		}

		return true;
	}

	bool InputHandler::UnregisterEach(const int buttonName, const PressType pressType)
	{
		bool ret = false;

		switch (pressType)
		{
		case PressType::PRESSED:
			ret = Unregister(buttonName, m_PressedFuncMap);
			break;
		case PressType::RELEASED:
			ret = Unregister(buttonName, m_ReleasedFuncMap);
			break;
		case PressType::PRESS:
			ret = Unregister(buttonName, m_PressFuncMap);
			break;
		}

		return ret;
	}

	void InputHandler::RegisterInputFunction(const int buttonName,const std::function<void()>& func,const PressType pressType)
	{
		switch (pressType)
		{
		case PressType::PRESSED:
			m_PressedFuncMap[buttonName] = func;
			break;
		case PressType::RELEASED:
			m_ReleasedFuncMap[buttonName] = func;
			break;
		case PressType::PRESS:
			m_PressFuncMap[buttonName] = func;
			break;
		}
	}

	void InputHandler::RegisterLStickDownFunction(const std::function<void(const StickDownDir stickDownDir)>& func)
	{
		m_LStickDownFunc = func;
	}

	bool InputHandler::UnregisterInputFunction(const int buttonName, const PressType pressType)
	{
		bool ret = IsRegistered(buttonName, pressType);

		if (ret)
		{
			m_WaitUnregister.push_back(pair<int,PressType>(buttonName,pressType));
			return ret;
		}

		return ret;
	}

	bool InputHandler::IsRegistered(const int buttonName, const PressType pressType) const
	{
		bool ret = false;

		switch (pressType)
		{
		case PressType::PRESSED:
			ret = CheckRegistered(buttonName, m_PressedFuncMap);
			break;
		case PressType::RELEASED:
			ret = CheckRegistered(buttonName, m_ReleasedFuncMap);
			break;
		case PressType::PRESS:
			ret = CheckRegistered(buttonName, m_PressFuncMap);
			break;
		}

		return ret;
	}

	void InputHandler::PushHandle()
	{
		//íœ‘Ò‚¿‚Ì•¨‚ðíœ
		for (auto p : m_WaitUnregister)
		{
			UnregisterEach(p.first, p.second);
		}

		m_WaitUnregister.clear();

		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		for (int i = 0; i < 4; i++)
		{
			if (cntVec[i].bConnected)
			{
				for (auto obj : m_PressedFuncMap)
				{
					if (cntVec[i].wPressedButtons & obj.first)
						obj.second();
				}

				for (auto obj : m_PressFuncMap)
				{
					if (cntVec[i].wButtons & obj.first)
						obj.second();
				}

				for (auto obj : m_ReleasedFuncMap)
				{
					if (cntVec[i].wReleasedButtons & obj.first)
						obj.second();
				}

				break;
			}
		}
	}

	void InputHandler::StickHandle(const float minStickLength)
	{
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		for (int i = 0; i < 4; i++)
		{
			if (cntVec[i].bConnected)
			{
				Vec2 l;
				l.x = cntVec[i].fThumbLX;
				l.y = cntVec[i].fThumbLY;

				Vec2 r;
				r.x = cntVec[i].fThumbRX;
				r.y = cntVec[i].fThumbRY;

				if (bsm::length(l) < minStickLength)
					m_LStickDir = Vec2(0, 0);
				else
					m_LStickDir = l;

				if (bsm::length(r) < minStickLength)
					m_RStickDir = Vec2(0, 0);
				else
					m_RStickDir = r;

				break;
			}
		}
	}

	void InputHandler::TriggerHandle(const float minTriggerLength)
	{
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		for (int i = 0; i < 4; i++)
		{
			if (cntVec[i].bConnected)
			{
				BYTE l = cntVec[i].bLeftTrigger;
				BYTE r = cntVec[i].bRightTrigger;

				float fl = l / 255.0f;
				float fr = r / 255.0f;

				if (fl > minTriggerLength)
					m_LTrigger = fl;
				else
					m_LTrigger = 0.0f;

				if (fr > minTriggerLength)
					m_RTrigger = fr;
				else
					m_RTrigger = 0.0f;

				break;
			}
		}
	}

	void InputHandler::LStickDownHandle()
	{
		if (!m_LStickDownFunc)
			return;

		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		for (int i = 0; i < 4; i++)
		{
			if (cntVec[i].bConnected)
			{
				Vec2 input;
				input.x = cntVec[i].fThumbLX;
				input.y = cntVec[i].fThumbLY;
				if (input.length() < 0.75f)
				{
					m_IsStickDown = false;
					m_LStickDownFunc(StickDownDir::NEUTRAL);
					return;
				}

				if (!m_IsStickDown)
				{
					if (abs(input.x) > abs(input.y))
					{
						//‰E
						if (input.x > 0)
							m_LStickDownFunc(StickDownDir::RIGHT);
						//¶
						else
							m_LStickDownFunc(StickDownDir::LEFT);
					}
					else if (abs(input.y) > abs(input.x))
					{
						//ã
						if (input.y > 0)
							m_LStickDownFunc(StickDownDir::UP);
						//‰º
						else
							m_LStickDownFunc(StickDownDir::DOWN);
					}

					m_IsStickDown = true;
				}
				else
					m_LStickDownFunc(StickDownDir::NEUTRAL);

				break;
			}
		}
	}
}