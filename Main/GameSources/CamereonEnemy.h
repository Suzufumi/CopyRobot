#pragma once
#include "stdafx.h"

namespace basecross
{
	class CamereonEnemy : public Enemy
	{
	private:
		unique_ptr<StateMachine<CamereonEnemy>> m_stateMachine;
		unique_ptr<StateMachine<CamereonEnemy>> m_camelAnimStateMachine;
	public:
		CamereonEnemy(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
			: Enemy(stagePtr, pNode)
		{}
		virtual ~CamereonEnemy()
		{
		}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		virtual void OnDraw() override;
		// �A�j���[�V�����̍X�V
		void UpdateAnimation() override;

		// �X�e�[�g�}�V�[���̎擾
		unique_ptr<StateMachine<CamereonEnemy>>& GetStateMachine()
		{ return m_stateMachine; }

		unique_ptr<StateMachine<CamereonEnemy>>& GetCamelAnimStateMachine()
		{ return m_camelAnimStateMachine; }

	};
}