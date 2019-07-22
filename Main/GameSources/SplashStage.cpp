#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void SplashStage::CreateViewLight()
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

	void SplashStage::OnCreate()
	{
		CreateViewLight();

		m_DisplayTex.push_back(L"CREDIT_1_TX");
		m_DisplayTex.push_back(L"CREDIT_2_TX");

		m_Sprite = AddGameObject<GameObject>();
		m_Sprite.lock()->GetComponent<Transform>()->SetScale(Vec3(1280, 800, 0));
		auto drawComp = m_Sprite.lock()->AddComponent<PTSpriteDraw>();
		drawComp->SetTextureResource(m_DisplayTex[0]);
	}

	void SplashStage::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		m_TimeCount += delta;

		auto transition = App::GetApp()->GetScene<Scene>()->GetTransition();

		auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (pad.bConnected && !transition->GetIsFadeNow())
		{
			if (pad.wPressedButtons)
			{
				m_TimeCount = m_Interval;
			}
		}

		if (m_TimeCount >= m_Interval)
		{
			m_TimeCount = 0.0f;
			auto transIn = ObjectFactory::Create<NormalFadeTransition>(1.0f, TransitionState::FadeIn,Col4(0,0,0,1));
			auto transOut = ObjectFactory::Create<NormalFadeTransition>(1.0f, TransitionState::FadeOut,Col4(0, 0, 0, 1));

			m_Index++;
			if (static_cast<unsigned>(m_Index) < m_DisplayTex.size())
			{
				transition->PlayInOutTransitionWithAction(transIn, transOut, [this]() {
					m_Sprite.lock()->GetComponent<PTSpriteDraw>()->SetTextureResource(m_DisplayTex[m_Index]);
				});
			}
			else
			{
				transition->PlayInOutTransitionWithMsg(transIn, transOut, L"ToTitle");
			}
		}
	}
}