#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TutorialStage1::OnCreate()
	{
		GameStage::OnCreate();

		m_StateMachine.reset(new StateMachine<TutorialStage1>(GetThis<TutorialStage1>()));
		m_StateMachine->ChangeState(Tutorial1NoneState::Instance());
	}

	void TutorialStage1::OnUpdate()
	{
		Stage::OnUpdate();
		m_EfkInterface->OnUpdate();
		m_StateMachine->Update();

	}

	void TutorialStage1::OnEvent(const shared_ptr<Event>& event)
	{
		GameStage::OnEvent(event);

		if (event->m_MsgStr == L"Tutorial1Start")
		{
			m_StateMachine->ChangeState(Tutorial1StartState::Instance());
		}
	}

	void TutorialStage1::ResetEnemies()
	{
		vector<shared_ptr<GameObject>> enemies;
		GetUsedTagObjectVec(L"Enemy", enemies);
		for (auto enemy : enemies)
		{
			dynamic_pointer_cast<Enemy>(enemy)->GetRedLineSprite().lock()->SetDiffuse(Col4(1, 1, 1, 0));
			RemoveGameObject<GameObject>(enemy->GetThis<GameObject>());
		}

		CreateEnemy();
	}

	IMPLEMENT_SINGLETON_INSTANCE(Tutorial1NoneState)

	IMPLEMENT_SINGLETON_INSTANCE(Tutorial1StartState)

	void Tutorial1StartState::Enter(const shared_ptr<TutorialStage1>& stage)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring stageXmlPath = dataDir + L"Stage\\" +  L"1.xml";

		Vec3 cameraMovePos;

		XmlDocReader xmlReader(stageXmlPath);
		auto wayNodes = xmlReader.GetSelectNodes(L"GameStage/CameraMove/WayPoint");
		long wayCounts = XmlDocReader::GetLength(wayNodes);
		if (wayCounts > 0)
		{
			for (long i = 0; i < wayCounts; i++)
			{
				auto node = XmlDocReader::GetItem(wayNodes, i);
				wstring posStr = XmlDocReader::GetAttribute(node, L"Pos");
				Vec3 pos = MyUtil::unityVec3StrToBCVec3(posStr);
				cameraMovePos = pos;
			}
		}

		auto cameraNode = xmlReader.GetSelectSingleNode(L"GameStage/CameraMove");
		wstring posStr = XmlDocReader::GetAttribute(cameraNode, L"Pos");
		Vec3 cameraStartPos = MyUtil::unityVec3StrToBCVec3(posStr);

		auto camera = stage->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			vector<shared_ptr<GameObject>> goals;
			tpsCam->SetEye(cameraStartPos);
			stage->GetUsedTagObjectVec(L"Goal", goals);
			tpsCam->SetTarget(goals[0]->GetComponent<Transform>());
			tpsCam->CloseUpTarget(cameraMovePos,3.0f);
		}

		m_CloseCount = 0.0f;

		vector<shared_ptr<GameObject>> player;
		stage->GetUsedTagObjectVec(L"Player", player);
		player[0]->SetUpdateActive(false);
	}

	void Tutorial1StartState::Execute(const shared_ptr<TutorialStage1>& stage)
	{
		m_CloseCount += App::GetApp()->GetElapsedTime();

		auto transition = App::GetApp()->GetScene<Scene>()->GetTransition();
		if (m_CloseCount >= 3.0f)
		{
			if (!transition->GetIsFadeNow())
			{
				auto transOut = ObjectFactory::Create<NormalFadeTransition>(0.9f, TransitionState::FadeOut, Col4(0, 0, 0, 1));
				transition->PlayFadeOut(transOut, true);
			}
		}

		if (m_CloseCount >= 4.0f)
		{
			auto transIn = ObjectFactory::Create<NormalFadeTransition>(1.5f, TransitionState::FadeIn, Col4(0, 0, 0, 1));
			transition->PlayFadeIn(transIn);
			stage->m_StateMachine->ChangeState(Tutorial1NoneState::Instance());
		}
	}

	void Tutorial1StartState::Exit(const shared_ptr<TutorialStage1>& stage)
	{
		vector<shared_ptr<GameObject>> player;
		stage->GetUsedTagObjectVec(L"Player", player);

		auto camera = stage->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->SetTarget(player[0]->GetComponent<Transform>());
			tpsCam->EndClose();
		}

		player[0]->SetUpdateActive(true);
	}
}