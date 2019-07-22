#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void SelectStage::CreateViewLight() 
	{
		auto PtrView = CreateView<SingleView>();
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 0.8f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		auto PtrMultiLight = CreateLight<MultiLight>();
		PtrMultiLight->SetDefaultLighting2();
		auto& L0 = PtrMultiLight->GetLight(0);
		auto& L1 = PtrMultiLight->GetLight(1);
		auto& L2 = PtrMultiLight->GetLight(2);

		L0.m_Directional = Vec3(1.0f, 1.0f, 1.0f);
		App::GetApp()->GetScene<Scene>()->SetClearColor(Col4(0, 0, 0, 1));
	}

	//スクロールスプライト作成
	void SelectStage::CreateScrollSprite() 
	{
		m_DrawOrder = AddGameObject<ScrollSprite>(L"BGSelect_TX", true,
			Vec2(1280.0f, 800.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_DrawOrder->SetDrawActive(false);
	}
	
	void SelectStage::CreateArrowSprite()
	{

		m_L_Arrow= AddGameObject<ArrowSprite>(L"L_ARROW_TX", true,
			Vec2(184.0f, 162.0f)*0.5f, Vec2(-570.0f, -45.0f));
		auto lArrowLk = m_L_Arrow.lock();
		lArrowLk->SetDrawLayer(100);
		lArrowLk->GetComponent<Action>()->AllActionClear();
		lArrowLk->GetComponent<Action>()->AddMoveBy(0.5f, Vec3(-16.0f, 0.0f, 0.0f));
		lArrowLk->GetComponent<Action>()->AddMoveBy(0.5f, Vec3(16.0f, 0.0f, 0.0f));
		//ループする
		lArrowLk->GetComponent<Action>()->SetLooped(true);
		lArrowLk->GetComponent<Action>()->Run();

		m_R_Arrow = AddGameObject<ArrowSprite>(L"R_ARROW_TX", true,
			Vec2(184.0f, 162.0f)*0.5f, Vec2(570.0f, -45.0f));
		auto rArrowLk = m_R_Arrow.lock();
		rArrowLk->SetDrawLayer(100);
		rArrowLk->GetComponent<Action>()->AllActionClear();
		rArrowLk->GetComponent<Action>()->AddMoveBy(0.5f, Vec3(16.0f, 0.0f, 0.0f));
		rArrowLk->GetComponent<Action>()->AddMoveBy(0.5f, Vec3(-16.0f, 0.0f, 0.0f));
		//ループする
		rArrowLk->GetComponent<Action>()->SetLooped(true);
		rArrowLk->GetComponent<Action>()->Run();


	}

	void SelectStage::CreateSelectCharacter()
	{

		AddGameObject<SelectMojiModel>(Vec3(0.0f, 0.7f, 0.0f));
	}

	void SelectStage::CreateSelectRing()
	{
		m_Ring = AddGameObject<SelectRingModel>(Vec3(0.0f, 0.0f, 0.0f));

	}

	void SelectStage::CreateStageSelectSquare()
	{
		PostSquare::InitParam SquareParam;
		SquareParam.m_Scale = Vec3(0.75f, 0.75f, 0.75f);
		SquareParam.m_AspectWdivH = 2.5f;
		SquareParam.m_Position = Vec3(0.0f,0.25f,1.0f);
		

		for (int i = 0; i < 8; i++) 
		{
			SquareParam.m_RenderTarget = App::GetApp()->GetScene<Scene>()->GetStageSelectStages()[i]->GetRenderTarget();
			SquareParam.m_Num = i + 1;
			m_Squares.push_back(AddGameObject<PostSquare>(SquareParam));
		}
		
		for (auto stageSelect : App::GetApp()->GetScene<Scene>()->GetStageSelectStages())
		{
			stageSelect->OnUpdate();
			stageSelect->UpdateStage();
		}
	}


	//初期化
	void SelectStage::OnCreate()
	{
		CreateViewLight();
		//スプライトの作成
		CreateScrollSprite();
		//CreateAnimeSprite();
		CreateArrowSprite();
		CreateSelectCharacter();
		CreateSelectRing();
		CreateStageSelectSquare();		
		auto PtrScene = App::GetApp()->GetScene<Scene>();
		PtrScene->SetStageNum(0);

	}

	void SelectStage::ChangeSelect(int num)
	{
		App::GetApp()->GetScene<Scene>()->SetStageNum(num);
	}


	//更新
	void SelectStage::OnUpdate()
	{
		//シーンの取得
		auto PtrScene = App::GetApp()->GetScene<Scene>();
		int StageNum = PtrScene->GetStageNum();
		auto Audioptr = App::GetApp()->GetXAudio2Manager();

		auto ringLk = m_Ring.lock();
		//コントローラの取得
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected)
		{
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A&&App::GetApp()->GetScene<Scene>()->GetTransition()->GetIsFadeNow() == false)
			{
				if (!m_IsStart)
				{ 
					m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
					auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
				    auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
					App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToGameStage");

					m_IsStart = true;
				}
			}
			if (!m_CntrolLock) 
			{
				if (ringLk->GetComponent<Action>()->IsArrived())
				{
					if (pad.fThumbLX >= 0.8f)
					{
						StageNum++;
						if (StageNum > 7)
						{
							StageNum = 0;
						}
						m_Audio = Audioptr->Start(L"Select2", XAUDIO2_NO_LOOP_REGION, 1.0f);
						m_CntrolLock = true;
						ringLk->GetComponent<Action>()->AllActionClear();
						ringLk->GetComponent<Action>()->AddRotateBy(1.0f, Vec3(0, XM_PIDIV4, 0));
						ringLk->GetComponent<Action>()->Run();
						PtrScene->SetStageNum(StageNum);
						ChangeSelect(StageNum);
					}
					else if (pad.fThumbLX <= -0.8f)
					{
						StageNum--;
						if (StageNum < 0)
						{
							StageNum = 7;
						}
						m_Audio = Audioptr->Start(L"Select2", XAUDIO2_NO_LOOP_REGION, 1.0f);
						m_CntrolLock = true;
						ringLk->GetComponent<Action>()->AllActionClear();
						ringLk->GetComponent<Action>()->AddRotateBy(1.0f, Vec3(0, -XM_PIDIV4, 0));
						ringLk->GetComponent<Action>()->Run();

						PtrScene->SetStageNum(StageNum);
						ChangeSelect(StageNum);
					}
				}
			}
			else 
			{
				if (pad.fThumbLX == 0.0f)
				{
					m_CntrolLock = false;
				}
			}
		}

		float diffRad = -XM_PIDIV4;
		for (size_t i = 0; i < m_Squares.size(); i++)
		{
			auto trans = m_Squares[i].lock()->GetComponent<Transform>();
			Quat qt;
			qt.rotationY(diffRad * i);
			trans->SetQuaternion(ringLk->GetComponent<Transform>()->GetQuaternion() * qt);
			Vec3 pos = trans->GetPosition();
			float rad = ringLk->GetComponent<Transform>()->GetRotation().y;
			pos.x = sinf(rad + diffRad * i) * -1.5f;
			pos.z = cosf(rad + diffRad * i) * -1.5f;
			trans->SetPosition(pos);
		}

		if (m_IsFirstUpdate)
		{
			m_IsFirstUpdate = false;
			for (auto stage : App::GetApp()->GetScene<Scene>()->GetStageSelectStages())
			{
				stage->OnDraw();
			}
		}

		auto stageSelect = App::GetApp()->GetScene<Scene>()->GetStageSelectStages()[StageNum];
		stageSelect->OnUpdate();
		stageSelect->UpdateStage();
		stageSelect->OnDraw();
	}

	void SelectStage::DrawStage()
	{
		m_DrawOrder->OnDraw();
		Stage::DrawStage();

	}
	void SelectStage::DrawStrings()
	{
	}
}