#pragma once
#include "stdafx.h"

namespace basecross
{
	class NormalEnemy : public Enemy
	{
	private:
		unique_ptr<StateMachine<NormalEnemy>> m_stateMachine;
		unique_ptr<StateMachine<NormalEnemy>> m_animStateMachine;
	public:
		NormalEnemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
			: Enemy(stagePtr, pNode)
		{}
		virtual ~NormalEnemy()
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		// アニメーションの更新
		void UpdateAnimation() override;

		unique_ptr<StateMachine<NormalEnemy>>& GetStateMachine()
		{ return m_stateMachine; }

		unique_ptr<StateMachine<NormalEnemy>>& GetAnimStateMachine()
		{ return m_animStateMachine; }
	};


}