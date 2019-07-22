#include"stdafx.h"
#include"Project.h"

namespace basecross
{
	//���\�[�X�̍쐬
	void Title::CreateResourses()
	{
		wstring DataDir;
	}

	//�X�v���C�g�쐬
	void Title::CreateWallSprite()
	{
	}

	//�r���[�ƃ��C�g�̍쐬
	void Title::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vec3(0.0f, 2.5f, -6.0f));
		PtrCamera->SetAt(Vec3(0.0f, 2.0f, 0.0f));
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	void Title::CreateTitle()
	{

	}

	//�^�C�g���A�j���[�V�����̍쐬
	void Title::CreateTitleAnime() 
	{
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		AddGameObject<SpriteStudioAnim>(DataDir + L"TitleAnimation\\");
	}


	Title::~Title() {}

	void Title::OnCreate()
	{

		try
		{

			//���\�[�X�̍쐬
			CreateResourses();
			//�X�v���C�g�쐬
			CreateWallSprite();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�^�C�g��
			CreateTitle();
			//�^�C�g���A�j���[�V�����̍쐬
			CreateTitleAnime();

			m_Player = AddGameObject<GameObject>();
			m_Player.lock()->SetDrawLayer(100);
			auto drawComp = m_Player.lock()->AddComponent<PNTBoneModelDraw>();
			drawComp->SetMeshResource(L"PLAYER_MESH");
			drawComp->SetLineTextureResource(L"PLAYER_LINE_TEX");
			drawComp->SetEmissive(Col4(0.4f, 0.4f, 0.4f, 0.0f));
			drawComp->SetSpecular(Col4(0.2f, 0.2f, 0.2f, 7.0f));
			drawComp->BoneInit();
			drawComp->AddAnimation(L"Default", 0, 119, true);
			drawComp->AddAnimation(L"OpenClose", 200, 119, false, 60.0f);
			drawComp->ChangeCurrentAnimation(L"Default");

		}
		catch (...)
		{
			throw;
		}

	}

	void Title::OnUpdate()
	{
		auto Audioptr = App::GetApp()->GetXAudio2Manager();
		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		auto ScenePtr = App::GetApp()->GetScene<Scene>();
		if (pad.wPressedButtons&&App::GetApp()->GetScene<Scene>()->GetTransition()->GetIsFadeNow()==false)
		{
			if (!m_IsStart)
			{
				m_Audio = Audioptr->Start(L"OK", XAUDIO2_NO_LOOP_REGION, 5.0f);
				auto transIn = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeIn, Col4(0, 0, 0, 1));
				auto transOut = ObjectFactory::Create<FadeMaskTransition>(0.8f, L"MASK_TX", TransitionState::FadeOut, Col4(0, 0, 0, 1));
				App::GetApp()->GetScene<Scene>()->GetTransition()->PlayInOutTransitionWithMsg(transIn, transOut, L"ToSelectStage");

				m_IsStart = true;
			}
		}
		float delta = App::GetApp()->GetElapsedTime();
		m_PlayerAnimCount += delta;
		m_Player.lock()->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);

		if (m_PlayerAnimCount > 10.0f)
		{
			auto drawComp = m_Player.lock()->GetComponent<PNTBoneModelDraw>();
			if (drawComp->GetCurrentAnimation() != L"OpenClose")
			{
				drawComp->ChangeCurrentAnimation(L"OpenClose");
			}
			else
			{
				if (drawComp->GetCurrentAnimationTime() >= 119.0f / 60.0f)
				{
					m_PlayerAnimCount = 0.0f;
					drawComp->ChangeCurrentAnimation(L"Default");
				}
			}
		}
	}
	void Title::OnDestroy()
	{
	}
}