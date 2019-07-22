/*!
@file PlayerStateMachine.cpp
@brief プレイヤーのステートマシン
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	IMPLEMENT_SINGLETON_INSTANCE(PlayerNoneState)

	//--------------------------------------------------------------------------------------
	///	ゲームオーバーステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDeadState)

	void PlayerDeadState::Enter(const shared_ptr<Player>& Obj) 
	{
		auto stage = dynamic_pointer_cast<GameStage>(Obj->GetStage());
		stage->StopAllEnemies();

		auto trans = Obj->GetComponent<Transform>();
		Obj->m_EfkPlay = ObjectFactory::Create<EfkPlay>(Obj->m_DownEffect, trans->GetPosition() + Vec3(0,-2.0f,0.0f));

		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			auto trans = Obj->GetComponent<Transform>();
			auto closePos = trans->GetPosition() + trans->Forward() * 10.0f + Vec3(0.0f, 0.0f, 0.0f);
			tpsCam->CloseUpTarget(closePos);
		}

		Obj->GetComponent<Rigidbody>()->SetUpdateActive(false);
		Obj->GetComponent<CollisionObb>()->SetUpdateActive(false);

		auto action = Obj->GetComponent<Action>();
		action->AllActionClear();
		action->AddMoveBy(0.75f, Vec3(0, -3.0f, 0.0f),Lerp::Easein);
		action->AddScaleTo(0.75f, Vec3(0.0f, 1.5f, 0.0f),Lerp::Easein);
		action->AddRotateTo(0.75f, Vec3(0, XM_2PI, 0.0f));
		action->AddRotateInterval(0.0f);
		action->Run();

		Obj->ChangeAlpha(1.0f);
		Obj->GetAssimililationObject()->ChangeAlpha(0.0f);

		Obj->m_CopySuccessGauge.lock()->RunCloseAction();
		Obj->m_UsageTimeGauge.lock()->RunCloseAction();

		vector<shared_ptr<GameObject>> miniMapVec;
		Obj->GetStage()->GetUsedTagObjectVec(L"MiniMap", miniMapVec);
		miniMapVec[0]->SetDrawActive(false);

		App::GetApp()->GetXAudio2Manager()->Start(L"WARP_SE", static_cast<size_t>(0.6f));

		App::GetApp()->GetXAudio2Manager()->Start(L"GAME_OVER_JINGLE", 0);
		Obj->PostEvent(0.0f, Obj->GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>()->GetThis<ObjectInterface>(), L"StopBGM");

		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		m_OverAnim = Obj->GetStage()->AddGameObject<SpriteStudioAnim>(dataDir + L"GameOverUI\\", L"Gameover.ssae", L"anime_1");
		m_OverAnim.lock()->SetLooped(false);
		m_OverAnim.lock()->SetDrawLayer(700);
	}

	void PlayerDeadState::Execute(const shared_ptr<Player>& Obj)
	{
		if (Obj->GetComponent<Action>()->IsArrived())
			Obj->m_StateMachine->ChangeState(PlayerNoneState::Instance());
	}

	void PlayerDeadState::Exit(const shared_ptr<Player>& Obj)
	{
		//チュートリアルステージだった場合はゲームオーバーステージに行かない
		auto dynStage = dynamic_pointer_cast<TutorialStage1>(Obj->GetStage());
		if (dynStage)
		{
			auto transIn = ObjectFactory::Create<FadeMaskTransition>(1.5f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
			auto transOut = ObjectFactory::Create<FadeMaskTransition>(1.5f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithAction(transIn, transOut, [Obj,dynStage,this]() {
				Obj->GetComponent<Transform>()->ResetPosition(Obj->GetStartPosition());
				Obj->GetStateMachine()->ChangeState(PlayerNormalState::Instance());
				Obj->GetComponent<Transform>()->SetScale(Vec3(1, 1, 1));
				Obj->GetComponent<Transform>()->SetQuaternion(Obj->GetStartQuat());

				Obj->GetComponent<Rigidbody>()->SetUpdateActive(true);
				Obj->GetComponent<CollisionObb>()->SetUpdateActive(true);

				vector<shared_ptr<GameObject>> miniMapVec;
				Obj->GetStage()->GetUsedTagObjectVec(L"MiniMap", miniMapVec);
				miniMapVec[0]->SetDrawActive(true);
				
				auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
				auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
				if (tpsCam)
				{
					tpsCam->EndClose();
				}

				dynStage->ResetEnemies();

				if (m_OverAnim.lock())
					Obj->GetStage()->RemoveGameObject<GameObject>(m_OverAnim.lock()->GetThis<GameObject>());

				Obj->PostEvent(0.0f, Obj->GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>()->GetThis<ObjectInterface>(), L"StartMainBGM");
			});
		}
		else
		{
			auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.5f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
			auto transOut = ObjectFactory::Create<FadeMaskTransition>(3.0f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
			App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameOver");
		}
	}

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerNormalState)

		void PlayerNormalState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_A, [Obj]() {
			auto enemies = Obj->GetNearObjectVec(L"Enemy");
			for (auto enemy : enemies)
			{
				if (Obj->CheckIsBehindEnemy(enemy->GetComponent<Transform>()))
				{
					enemy->SetUpdateActive(false);
					Obj->GetBehavior<PlayerBehaviour>()->SetCopyTarget(enemy);
					if (Obj->GetBehavior<PlayerBehaviour>()->GetCopyTarget() != enemy)
						Obj->SetCopySuccessValue(0.0f);

					Obj->GetStateMachine()->ChangeState(PlayerCopyingState::Instance());
					return;
				}
			}

			auto staticObjects = Obj->GetNearObjectVec(L"CopyObj");
			if (staticObjects.size() > 0)
			{
				staticObjects[0]->SetUpdateActive(false);
				Obj->GetBehavior<PlayerBehaviour>()->SetCopyTarget(staticObjects[0]);
				if (Obj->GetBehavior<PlayerBehaviour>()->GetCopyTarget() != staticObjects[0])
					Obj->SetCopySuccessValue(0.0f);

				Obj->GetStateMachine()->ChangeState(PlayerCopyingState::Instance());
				return;
			}

		}, InputHandler::PressType::PRESSED);

		Obj->m_NowCopyType = Player::CopyType::Normal;

		Obj->m_UsageTimeGauge.lock()->RunCloseAction();
	}

	void PlayerNormalState::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetBehavior<PlayerBehaviour>()->Move();
		Obj->SetCursorToCopyableObject();

		float successValue = Obj->GetCopySuccessValue();
		if (successValue > 0.0f)
		{	
			successValue -= App::GetApp()->GetElapsedTime();;
			if (successValue < 0.0f)
				successValue = 0.0f;

			Obj->SetCopySuccessValue(successValue);
		}
	}

	void PlayerNormalState::Exit(const shared_ptr<Player>& Obj) {
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_A, []() {}, InputHandler::PressType::PRESSED);
		Obj->GetComponent<Rigidbody>()->SetVelocity(Vec3(0, 0, 0));
		Obj->m_TargetCursor.lock()->SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	///	コピーステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCopyingState)

	void PlayerCopyingState::Enter(const shared_ptr<Player>& Obj) 
	{
		Obj->GetBehavior<PlayerBehaviour>()->SetPressTime(Obj->GetCopySuccessValue());

		Obj->GetAnimationStateMachine()->ChangeState(PlayerCopyOpenAnim::Instance());

		Obj->Move(Vec3(0, 0, 0));

		Obj->m_UsageTimeGauge.lock()->RunCloseAction();

		m_PrevType = Obj->m_NowCopyType;
	}

	void PlayerCopyingState::Execute(const shared_ptr<Player>& Obj)
	{
		auto Type = Obj->GetBehavior<PlayerBehaviour>()->Copy();
		switch (Type)
		{
		case Player::CopyType::None:
			break;
		case Player::CopyType::Normal:
			Obj->GetAnimationStateMachine()->ChangeState(PlayerCopyCloseAnim::Instance());
			if (Obj->GetStateMachine()->GetPreviousState() == PlayerCamereonDefaultState::Instance())
			{
				Obj->GetStateMachine()->ChangeState(PlayerCamereonDefaultState::Instance());
				Type = m_PrevType;
			}
			else
				Obj->GetStateMachine()->ChangeState(PlayerNormalState::Instance());

			break;
		case Player::CopyType::NormalEnemy:
			Obj->GetAssimililationObject()->ChangeMeshResource(L"ENEMY_MESH");
			Obj->GetStateMachine()->ChangeState(PlayerTakeInState::Instance());
			break;
		case Player::CopyType::CamereonEnemy:
			Obj->GetAssimililationObject()->ChangeMeshResource(L"CAMELEON_MESH");
			Obj->GetStateMachine()->ChangeState(PlayerTakeInState::Instance());
			break;
		case Player::CopyType::Pillar:
			Obj->GetAssimililationObject()->ChangeMeshResource(L"PILLAR_BONE_MESH");
			Obj->GetStateMachine()->ChangeState(PlayerTakeInState::Instance());
			break;
		case Player::CopyType::CrystalTable:
			Obj->GetAssimililationObject()->ChangeMeshResource(L"CRYSTALTABLE_MESH");
			Obj->GetStateMachine()->ChangeState(PlayerTakeInState::Instance());
			break;
		}

		Obj->m_NowCopyType = Type;

		Obj->SetCursorToCopyableObject();
	}

	void PlayerCopyingState::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->m_TargetCursor.lock()->SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	///	コアにエネミーを取り込むステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerTakeInState)

	void PlayerTakeInState::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetBehavior<PlayerBehaviour>()->TakeInEnter();
	}

	void PlayerTakeInState::Execute(const shared_ptr<Player>& Obj)
	{
		auto Type = Obj->GetBehavior<PlayerBehaviour>()->TakeIn();
		Obj->SetCursorToCopyableObject();
		switch (Type)
		{
		case Player::CopyType::NormalEnemy:
		case Player::CopyType::CamereonEnemy:
		case Player::CopyType::Pillar:
		case Player::CopyType::CrystalTable:
			Obj->GetStateMachine()->ChangeState(PlayerCamereonDefaultState::Instance());
			break;
		}

		Obj->SetCursorToCopyableObject();
	}

	void PlayerTakeInState::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->GetAnimationStateMachine()->ChangeState(PlayerCopyCloseAnim::Instance());
		Obj->GetTargetCursor()->SetDrawActive(false);
		Obj->m_TargetCursor.lock()->SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	///	カメレオンステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCamereonDefaultState)

	void PlayerCamereonDefaultState::Enter(const shared_ptr<Player>& Obj) 
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, [Obj]() {
			Obj->GetStateMachine()->ChangeState(PlayerCamereonTransform::Instance());
		}, InputHandler::PressType::PRESSED);

		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_A, [Obj]() {
			auto enemies = Obj->GetNearObjectVec(L"NormalEnemy");
			for (auto enemy : enemies)
			{
				if (Obj->CheckIsBehindEnemy(enemy->GetComponent<Transform>()))
				{
					enemy->SetUpdateActive(false);
					Obj->GetBehavior<PlayerBehaviour>()->SetCopyTarget(enemy);
					if (Obj->GetBehavior<PlayerBehaviour>()->GetCopyTarget() != enemy)
						Obj->SetCopySuccessValue(0.0f);

					Obj->GetStateMachine()->ChangeState(PlayerCopyingState::Instance());
					return;
				}
			}

			enemies = Obj->GetNearObjectVec(L"CamereonEnemy");
			for (auto enemy : enemies)
			{
				if (Obj->CheckIsBehindEnemy(enemy->GetComponent<Transform>()))
				{
					enemy->SetUpdateActive(false);
					Obj->GetBehavior<PlayerBehaviour>()->SetCopyTarget(enemy);
					if (Obj->GetBehavior<PlayerBehaviour>()->GetCopyTarget() != enemy)
						Obj->SetCopySuccessValue(0.0f);

					Obj->GetStateMachine()->ChangeState(PlayerCopyingState::Instance());
					return;
				}
			}

			auto staticObjects = Obj->GetNearObjectVec(L"CopyObj");
			if (staticObjects.size() > 0)
			{
				staticObjects[0]->SetUpdateActive(false);
				Obj->GetBehavior<PlayerBehaviour>()->SetCopyTarget(staticObjects[0]);
				if (Obj->GetBehavior<PlayerBehaviour>()->GetCopyTarget() != staticObjects[0])
					Obj->SetCopySuccessValue(0.0f);

				Obj->GetStateMachine()->ChangeState(PlayerCopyingState::Instance());
				return;
			}
		}, InputHandler::PressType::PRESSED);

		Obj->m_UsageTimeGauge.lock()->RunOpenAction();
	}

	void PlayerCamereonDefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetBehavior<PlayerBehaviour>()->Move();

		float successValue = Obj->GetCopySuccessValue();
		if (successValue > 0.0f)
		{
			successValue -= App::GetApp()->GetElapsedTime();;
			if (successValue < 0.0f)
				successValue = 0.0f;

			Obj->SetCopySuccessValue(successValue);
		}

		Obj->SetCursorToCopyableObject();
	}

	void PlayerCamereonDefaultState::Exit(const shared_ptr<Player>& Obj) {
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, []() {}, InputHandler::PressType::PRESSED);
		Obj->GetInputHandler()->UnregisterInputFunction(XINPUT_GAMEPAD_A, InputHandler::PressType::PRESSED);

		Obj->m_TargetCursor.lock()->SetDrawActive(false);
	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCamereonTransform)

	void PlayerCamereonTransform::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, [Obj]() {
			Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
		}, InputHandler::PressType::RELEASED);

		Obj->GetDynamicComponent<SmBaseDraw>()->SetDepthStencilState(DepthStencilState::Read);

		m_Se = App::GetApp()->GetXAudio2Manager()->Start(L"TRANSFORM_UP_SE", 0);
	}

	void PlayerCamereonTransform::Execute(const shared_ptr<Player>& Obj)
	{
		Obj->GetBehavior<PlayerBehaviour>()->Move();

		if (Obj->DecreaseCopyValue() <= 0)
		{
			Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
			return;
		}

		if (Obj->GetBehavior<PlayerCamereonBehaviour>()->Transform())
		{
			Obj->GetStateMachine()->ChangeState(PlayerCamereonAssimililation::Instance());
			return;
		}

		float successValue = Obj->GetCopySuccessValue();
		if (successValue > 0.0f)
		{
			successValue -= App::GetApp()->GetElapsedTime();;
			if (successValue < 0.0f)
				successValue = 0.0f;

			Obj->SetCopySuccessValue(successValue);
		}

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected)
		{
			if(!(pad.wButtons & XINPUT_GAMEPAD_X))
				Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
		}
	}

	void PlayerCamereonTransform::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, []() {}, InputHandler::PressType::RELEASED);
		Obj->GetDynamicComponent<SmBaseDraw>()->SetDepthStencilState(DepthStencilState::Default);

		if(m_Se->m_SourceVoice)
			m_Se->m_SourceVoice->Stop();
	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCamereonAssimililation)

	void PlayerCamereonAssimililation::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, [Obj]() {
			Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
		}, InputHandler::PressType::RELEASED);

		auto Type = Obj->GetPlayerCopytype();
		switch (Type)
		{
		case Player::CopyType::None:
			break;
		case Player::CopyType::Normal:

			break;
		case Player::CopyType::NormalEnemy:

			break;
		case Player::CopyType::CamereonEnemy:
			Obj->GetComponent<CollisionObb>()->SetMakedScale(Vec3(1, 1, 2.5f));
			Obj->GetComponent<CollisionObb>()->SetPositionDiff(Vec3(0, 0, 0));
			break;
		case Player::CopyType::Pillar:

			break;
		case Player::CopyType::CrystalTable:
			Obj->GetComponent<CollisionObb>()->SetMakedScale(Vec3(0.5f, 2, 0.5f));
			break;
		}
	}

	void PlayerCamereonAssimililation::Execute(const shared_ptr<Player>& Obj)
	{
		Obj->GetBehavior<PlayerBehaviour>()->Move();
		if (Obj->DecreaseCopyValue() <= 0.0f)
		{
			Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
			return;
		}

		float successValue = Obj->GetCopySuccessValue();
		if (successValue > 0.0f)
		{
			successValue -= App::GetApp()->GetElapsedTime();;
			if (successValue < 0.0f)
				successValue = 0.0f;

			Obj->SetCopySuccessValue(successValue);
		}

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected)
		{
			if (!(pad.wButtons & XINPUT_GAMEPAD_X))
				Obj->GetStateMachine()->ChangeState(PlayerCamereonRelease::Instance());
		}
	}

	void PlayerCamereonAssimililation::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, []() {}, InputHandler::PressType::RELEASED);

		Obj->GetDynamicComponent<SmBaseDraw>()->SetDepthStencilState(DepthStencilState::Default);

		auto collPtr = Obj->GetComponent<CollisionObb>();
		collPtr->SetMakedScale(Vec3(1, 2, 1));
		collPtr->SetPositionDiff(Vec3(0, 0.5f, 0));
	}

	IMPLEMENT_SINGLETON_INSTANCE(PlayerCamereonRelease)

	void PlayerCamereonRelease::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, [Obj]() {
			if (Obj->GetCopySuccessValue() <= 0.0f)
				return;

			Obj->GetStateMachine()->ChangeState(PlayerCamereonTransform::Instance());
		}, InputHandler::PressType::PRESSED);

		Obj->GetDynamicComponent<SmBaseDraw>()->SetDepthStencilState(DepthStencilState::Read);

		m_Se = App::GetApp()->GetXAudio2Manager()->Start(L"TRANSFORM_DOWN_SE", 0);
	}

	void PlayerCamereonRelease::Execute(const shared_ptr<Player>& Obj)
	{
		Obj->GetBehavior<PlayerBehaviour>()->Move();
		float copyValue = Obj->DecreaseCopyValue();

		if (Obj->GetBehavior<PlayerCamereonBehaviour>()->CancelTransform())
		{
			if (copyValue > 0)
				Obj->GetStateMachine()->ChangeState(PlayerCamereonDefaultState::Instance());
			else
				Obj->GetStateMachine()->ChangeState(PlayerNormalState::Instance());

			return;
		}

		float successValue = Obj->GetCopySuccessValue();
		if (successValue > 0.0f)
		{
			successValue -= App::GetApp()->GetElapsedTime();
			if (successValue < 0.0f)
				successValue = 0.0f;

			Obj->SetCopySuccessValue(successValue);
		}
	}

	void PlayerCamereonRelease::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->GetInputHandler()->RegisterInputFunction(XINPUT_GAMEPAD_X, []() {}, InputHandler::PressType::PRESSED);

		Obj->GetDynamicComponent<SmBaseDraw>()->SetDepthStencilState(DepthStencilState::Default);

		if (m_Se->m_SourceVoice)
			m_Se->m_SourceVoice->Stop();
	}

	//--------------------------------------------------------------------------------------
	///	ゴール状態
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerGoalState)

	void PlayerGoalState::Enter(const shared_ptr<Player>& Obj)
	{
		auto stage = dynamic_pointer_cast<GameStage>(Obj->GetStage());
		stage->StopAllEnemies();

		auto trans = Obj->GetComponent<Transform>();

		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->CloseUpTarget(trans->GetPosition() + trans->Forward() * 8.0f + Vec3(0,2,0));
		}

		Obj->GetComponent<Rigidbody>()->SetUpdateActive(false);
		Obj->GetComponent<CollisionObb>()->SetUpdateActive(false);

		Obj->GetAnimationStateMachine()->ChangeState(PlayerGoalAnim::Instance());

		auto action = Obj->GetComponent<Action>();
		action->AllActionClear();
		action->AddMoveInterval(89.0f / 45.0f);
		action->AddMoveTo(0.7f, Obj->m_GoalPos, Lerp::Linear);
		action->AddMoveInterval(0.0f);
		action->AddMoveTo(0.5f, Obj->m_GoalPos + Vec3(0, 15.0f, 0), Lerp::Easein);
		action->Run();

		Obj->ChangeAlpha(1.0f);
		Obj->GetAssimililationObject()->ChangeAlpha(0.0f);

		Obj->m_CopySuccessGauge.lock()->RunCloseAction();
		Obj->m_UsageTimeGauge.lock()->RunCloseAction();

		vector<shared_ptr<GameObject>> miniMapVec;
		Obj->GetStage()->GetUsedTagObjectVec(L"MiniMap", miniMapVec);
		miniMapVec[0]->SetDrawActive(false);

		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		auto anim = Obj->GetStage()->AddGameObject<SpriteStudioAnim>(dataDir + L"ClearAnimation\\",L"SC2.ssae",L"anime_1");
		anim->SetLooped(false);
		anim->SetDrawLayer(700);
	}

	void PlayerGoalState::Execute(const shared_ptr<Player>& Obj)
	{
		if (Obj->GetComponent<Action>()->IsArrived())
		{
			Obj->m_StateMachine->ChangeState(PlayerNoneState::Instance());
		}
	}
	
	void PlayerGoalState::Exit(const shared_ptr<Player>& Obj)
	{
		auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.5f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
		auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.5f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
		App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameClear");
	}

	//--------------------------------------------------------------------------------------
	///	スタート状態
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerStartState)

	void PlayerStartState::Enter(const shared_ptr<Player>& Obj)
	{
		auto trans = Obj->GetComponent<Transform>();

		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->Fix(trans->GetPosition() - trans->Forward() * 7.0f + Vec3(0, 2, 0), trans->GetPosition()+Vec3(0,1,0));
		}

		Obj->SetDrawActive(false);

		App::GetApp()->GetXAudio2Manager()->Start(L"EGG_APPEAR_SE", 0,0.6f);
		m_IsStartOpenSe = false;
	}

	void PlayerStartState::Execute(const shared_ptr<Player>& Obj)
	{
		auto drawComp = Obj->m_Egg.lock()->GetDynamicComponent<SmBaseDraw>();
		if (drawComp->GetCurrentAnimationTime() >= 120.0f / 40.0f)
		{
			Obj->m_StateMachine->ChangeState(PlayerNormalState::Instance());
		}

		if (drawComp->GetCurrentAnimationTime() >= 2.0f)
		{
			if (!m_IsStartOpenSe)
			{
				m_IsStartOpenSe = true;
				App::GetApp()->GetXAudio2Manager()->Start(L"EGG_OPEN_SE", 0);
			}
			Obj->SetDrawActive(true);
		}
	}

	void PlayerStartState::Exit(const shared_ptr<Player>& Obj)
	{
		vector<shared_ptr<GameObject>> miniMapVec;
		Obj->GetStage()->GetUsedTagObjectVec(L"MiniMap",miniMapVec);
		miniMapVec[0]->SetDrawActive(true);

		App::GetApp()->GetXAudio2Manager()->Start(L"START_SE", 0);

		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCam = dynamic_pointer_cast<TpsCamera>(camera);
		if (tpsCam)
		{
			tpsCam->EndClose();
		}

		Obj->PostEvent(0.0f, Obj->GetThis<ObjectInterface>(), Obj->GetStage()->GetThis<ObjectInterface>(), L"Tutorial1Start");
	}
}
