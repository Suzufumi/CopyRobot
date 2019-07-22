#include"stdafx.h"
#include"Project.h"
#include "Over.h"

namespace basecross
{
	//リソースの作成
	void Over::CreateResourses()
	{
		wstring DataDir;
	}

	//スプライト作成
	void Over::CreateWallSprite()
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		auto anim = AddGameObject<SpriteStudioAnim>(dataDir + L"GameOverAnim\\", L"go1.ssae", L"anime_1");

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1, 1, 1),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0),
			Vec3(0, -1.0f, 0)
		);

		m_Player = AddGameObject<GameObject>();
		m_Player.lock()->SetDrawLayer(100);
		m_Player.lock()->GetComponent<Transform>()->SetPosition(Vec3(-0.3f, 1.25f, 0));
		m_Player.lock()->GetComponent<Transform>()->SetRotation(Vec3(0, 0, -XM_PIDIV4));
		auto drawComp = m_Player.lock()->AddComponent<PNTBoneModelDraw>();
		drawComp->SetMeshResource(L"PLAYER_MESH");
		drawComp->SetLineTextureResource(L"PLAYER_LINE_TEX");
		drawComp->SetEmissive(Col4(0.4f, 0.4f, 0.4f, 0.0f));
		drawComp->SetSpecular(Col4(0.2f, 0.2f, 0.2f, 7.0f));
		drawComp->BoneInit();
		drawComp->SetMeshToTransformMatrix(spanMat);
		
		auto action = m_Player.lock()->AddComponent<Action>();
		action->AddRotateTo(0.75f, Vec3(0, 0, -XM_PIDIV2));
		action->AddRotateTo(0.75f, Vec3(0, 0, -XM_PIDIV4));
		action->SetLooped(true);
		action->Run();
	}

	//ビューとライトの作成
	void Over::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vec3(0.0f, 2.5f, -6.0f));
		PtrCamera->SetAt(Vec3(0.0f, 2.0f, 0.0f));
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void Over::CreateAnimeSprite()
	{
		auto PtrSp = AddGameObject<AnimeSprite>(L"Retry_TX", true,
			Vec2(150.0f, 70.0f), Vec2(445.0f, -160.0f));
		PtrSp->SetSelect(true);
		m_SpVec[0] = PtrSp;
		
		PtrSp = AddGameObject<AnimeSprite>(L"TitleButton_TX", true,
			Vec2(150.0f, 70.0f), Vec2(445.0f, -260.0f));
		m_SpVec[1] = PtrSp;

		auto Select = AddGameObject<AnimeSprite>(L"STAGE_LINE_TX", true,
			Vec2(50.0f, 50.0f), Vec2(330.0f, SelectPosY));
		Select->AddComponent<Action>()->AddRotateBy(0.5f, Vec3(0.0f, 0.0f, 30.0f));
		Select->AddComponent<Action>()->Run();
		Select->AddComponent<Action>()->SetLooped(true);
		m_Cursor[0] = Select;
 
	}

	void Over::CreateOver()
	{
	}

	Over::~Over() {}

	void Over::OnCreate()
	{

		try
		{

			//リソースの作成
			CreateResourses();
			//スプライト作成
			CreateWallSprite();
			//ビューとライトの作成
			CreateViewLight();
			//ゲームオーバー
			CreateOver();
			//選択肢
			CreateAnimeSprite();

			m_InputHandler.reset(new InputHandler());

			m_InputHandler->RegisterLStickDownFunction(BC_CALLBACK_1(Over::OnLStickDown, this));
		}
		catch (...)
		{
			throw;
		}

	}

	void Over::ChangeSelect(int num)
	{
		for (int i = 0; i < 2; i++) {
			shared_ptr<AnimeSprite> shptr = m_SpVec[i].lock();
			if (shptr) {
				if ((i + 1 ) == num) {
					shptr->SetSelect(true);
				}
				else {
					shptr->SetSelect(false);
				}
			}
		}
	}

	void Over::OnUpdate()
	{
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
				SelectPosY = -260;
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A) {
					m_IsStart = true;
					cuptr->AddComponent<Action>()->AllActionClear();
					m_Audio = Audioptr->Start(L"NO", XAUDIO2_NO_LOOP_REGION, 1.5f);
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
					auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToTitle");
					return;
				}
				break;

			case 2:
				SelectPosY = -160;
				if (pad.wPressedButtons & XINPUT_GAMEPAD_A) {
					m_IsStart = true;
					m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
					cuptr->AddComponent<Action>()->AddRotateBy(0.5f, Vec3(0.0f, 0.0f, 60.0f));
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
					auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameStage");
					return;

				}
				break;
			}
		}
		cuptr->GetComponent<Transform>()->SetPosition(330.0f, SelectPosY, 0.0f);

	}

	void Over::OnLStickDown(InputHandler::StickDownDir stickDownDir)
	{
		auto Audioptr = App::GetApp()->GetXAudio2Manager();
		if (!m_IsStart)
		{
			switch (stickDownDir)
			{
			case InputHandler::StickDownDir::UP:
			{
				//スティックを上に倒したときの処理
				m_Audio = Audioptr->Start(L"Select", XAUDIO2_NO_LOOP_REGION, 3.0f);
				if (SelectNo <= 1)
				{
					SelectNo++;
				}
				else
				{
					SelectNo--;
				}
			}
			break;
			case InputHandler::StickDownDir::DOWN:
			{
				//スティックを下に倒したときの処理
				m_Audio = Audioptr->Start(L"Select", XAUDIO2_NO_LOOP_REGION, 3.0f);
				if (SelectNo >= 2)
				{
					SelectNo--;
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