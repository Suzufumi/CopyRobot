#pragma once
#include "stdafx.h"

namespace basecross
{
	class TutorialStage1 : public GameStage
	{
		friend class Tutorial1StartState;

		unique_ptr<StateMachine<TutorialStage1>> m_StateMachine;
	public:
		TutorialStage1():GameStage(){}
		virtual ~TutorialStage1() {}

		void ResetEnemies();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};

	class Tutorial1NoneState : public ObjState<TutorialStage1>
	{
		Tutorial1NoneState(){}
	public:
		DECLARE_SINGLETON_INSTANCE(Tutorial1NoneState);

		virtual void Enter(const shared_ptr<TutorialStage1>& stage) override {};
		virtual void Execute(const shared_ptr<TutorialStage1>& stage)override {};
		virtual void Exit(const shared_ptr<TutorialStage1>& stage)override {};
	};

	class Tutorial1StartState : public ObjState<TutorialStage1>
	{
		Tutorial1StartState() {}

		float m_CloseCount = 0.0f;
	public:
		DECLARE_SINGLETON_INSTANCE(Tutorial1StartState);

		virtual void Enter(const shared_ptr<TutorialStage1>& stage) override;
		virtual void Execute(const shared_ptr<TutorialStage1>& stage)override;
		virtual void Exit(const shared_ptr<TutorialStage1>& stage)override;
	};
}