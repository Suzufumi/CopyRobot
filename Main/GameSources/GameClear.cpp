#include"stdafx.h"
#include"Project.h"
#include "GameClear.h"

namespace basecross
{
	//リソースの作成
	void GameClear::CreateResourses()
	{
		wstring DataDir;
	}

	//スプライト作成
	void GameClear::CreateWallSprite()
	{
		m_WallSprite = AddGameObject<WallSprite>(L"Clear_TX", false,
			Vec2(1280.0f, 800.0f), Vec3(0.0f, 0.0f, 0.1f));
		m_WallSprite.lock()->SetDrawActive(false);
	}

	//ビューとライトの作成
	void GameClear::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vec3(0.0f, 1.75f, -5.0f));
		PtrCamera->SetAt(Vec3(0.0f, 1.75f, 0.0f));
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void GameClear::CreateAnimeSprite()
	{
		auto PtrSp = AddGameObject<AnimeSprite>(L"Result_Next_TX", true,
			Vec2(170.0f, 70.0f), Vec2(350.0f, 0.0f));
		PtrSp->SetSelect(true);
		m_SpVec[0] = PtrSp;

		PtrSp = AddGameObject<AnimeSprite>(L"Result_Retry_TX", true,
			Vec2(170.0f, 70.0f), Vec2(350.0f, -150.0f));
		m_SpVec[1] = PtrSp;
		PtrSp = AddGameObject<AnimeSprite>(L"Result_Title_TX", true,
			Vec2(170.0f, 70.0f), Vec2(350.0f, -300.0f));
		m_SpVec[2] = PtrSp;

		auto Select = AddGameObject<AnimeSprite>(L"STAGE_LINE_TX", true,
			Vec2(50.0f, 50.0f), Vec2(230.0f, SelectPosY));
		m_Cursor[0] = Select;
		Select->AddComponent<Action>()->AddRotateBy(0.5f, Vec3(0.0f, 0.0f, 30.0f));
		Select->AddComponent<Action>()->Run();
		Select->AddComponent<Action>()->SetLooped(true);
	}

	void GameClear::CreateClear()
	{

	}

	GameClear::~GameClear() {}

	void GameClear::OnCreate()
	{

		try
		{

			//リソースの作成
			CreateResourses();
			//スプライト作成
			CreateWallSprite();
			//ビューとライトの作成
			CreateViewLight();
			//ゲームクリア
			CreateClear();
			//選択肢
			CreateAnimeSprite();

			m_Player = AddGameObject<GameObject>();
			m_Player.lock()->SetDrawLayer(100);
			m_Player.lock()->GetComponent<Transform>()->SetPosition(Vec3(-1, 0, 0));
			auto drawComp = m_Player.lock()->AddComponent<PNTBoneModelDraw>();
			drawComp->SetMeshResource(L"PLAYER_MESH");
			drawComp->SetLineTextureResource(L"PLAYER_LINE_TEX");
			drawComp->SetEmissive(Col4(0.4f, 0.4f, 0.4f, 0.0f));
			drawComp->SetSpecular(Col4(0.2f, 0.2f, 0.2f, 7.0f));
			drawComp->BoneInit();
			drawComp->AddAnimation(L"Default", 950, 169, true);
			drawComp->AddAnimation(L"OpenClose", 200, 119, false, 60.0f);
			drawComp->ChangeCurrentAnimation(L"Default");

			m_InputHandler.reset(new InputHandler());
			m_InputHandler->RegisterLStickDownFunction(BC_CALLBACK_1(GameClear::OnLStickDown, this));
		}
		catch (...)
		{
			throw;
		}

	}

	void GameClear::ChangeSelect(int num)
	{
		for (int i = 0; i < 3; i++) {
			shared_ptr<AnimeSprite> shptr = m_SpVec[i].lock();
			if (shptr) {
				if ((i + 1) == num) {
					shptr->SetSelect(false);
				}
				else {
					shptr->SetSelect(true);
				}
			}
		}
	}

	void GameClear::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		m_Player.lock()->GetDynamicComponent<SmBaseDraw>()->UpdateAnimation(delta);

		m_InputHandler->LStickDownHandle();
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		auto ScenePtr = App::GetApp()->GetScene<Scene>();
		int StageNum = ScenePtr->GetStageNum();
		shared_ptr<AnimeSprite> cuptr = m_Cursor[0].lock();
		auto Audioptr = App::GetApp()->GetXAudio2Manager();
		if (!m_IsStart)
		{
			switch (SelectNo)
			{
			case 1:
				SelectPosY = 0.0f;
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A&&App::GetApp()->GetScene<Scene>()->GetTransition()->GetIsFadeNow() == false) {
					m_IsStart = true;
					m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
					StageNum++;
					if (StageNum > 7)
						StageNum = 0;
					ScenePtr->SetStageNum(StageNum);
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
					auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameStage");
					return;
				}
				break;

			case 2:
				SelectPosY = -150.0f;
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A&&App::GetApp()->GetScene<Scene>()->GetTransition()->GetIsFadeNow() == false) {
					m_IsStart = true;
					m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
					ScenePtr->SetStageNum(StageNum);
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
					auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameStage");
					return;
				}
				break;

			case 3:
				SelectPosY = -300.0f;
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A&&App::GetApp()->GetScene<Scene>()->GetTransition()->GetIsFadeNow() == false) {
					m_IsStart = true;
					m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
					auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToTitle");
					return;
					break;
				}
			}
		}
		cuptr->GetComponent<Transform>()->SetPosition(230.0f, SelectPosY, 0.0f);
	}

	void GameClear::DrawStage()
	{
		m_WallSprite.lock()->OnDraw();
		Stage::DrawStage();
	}

	void GameClear::OnLStickDown(InputHandler::StickDownDir stickDownDir)
	{
		auto Audioptr = App::GetApp()->GetXAudio2Manager();
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (!m_IsStart)
		{
			switch (stickDownDir)
			{
			case InputHandler::StickDownDir::UP:
			{
				m_Audio = Audioptr->Start(L"Select", XAUDIO2_NO_LOOP_REGION, 3.0f);
				//スティックを上に倒したときの処理
				if (SelectNo <= 1)
				{
					SelectNo = 3;
				}
				else
				{
					SelectNo--;
				}

			}
			break;
			case InputHandler::StickDownDir::DOWN:
			{
				m_Audio = Audioptr->Start(L"Select", XAUDIO2_NO_LOOP_REGION, 3.0f);
				//スティックを下に倒したときの処理
				if (SelectNo > 2)
				{
					SelectNo = 1;
				}
				else
				{
					SelectNo++;
				}
			}
			break;
			}
		}
			ChangeSelect(SelectNo);
	}
}