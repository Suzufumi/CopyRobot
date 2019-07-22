#pragma once
#include "stdafx.h"

namespace basecross
{
	class TutorialBox : public GameObject
	{
	public:
		struct InitParam
		{
			Vec3 m_Position;
			Vec3 m_Scale;
			Quat m_Quat;
			wstring m_TexKey;
		};
	private:
		InitParam m_InitParam;

	public:
		TutorialBox(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~TutorialBox(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec)override;
	};
}